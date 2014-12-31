#include "danmakuengine.h"
#include "bulletwidget.h"
#define DANMAKU_SERVER "http://vpn.wuqiong.tk:5000/"
#define DANMAKU_SERVER1 "http://dm.tuna.moe/"
#define DANMAKU_CHANNEL "diveinedu"


DanmakuEngine::DanmakuEngine(QObject *parent) :
    QObject(parent),
    danmakuQueue(new QQueue<Danmaku_Msg>()),
    danmakuTimer(NULL),
    settings(NULL)
{
    qRegisterMetaType<Danmaku_Msg>("Danmaku_Msg");

    this->settings = new QSettings();
    this->_server = this->settings->value("server").toString();
    this->_channel = this->settings->value("channel").toString();
    this->_password = this->settings->value("password").toString();

    DanmakuTray *systemTray=DanmakuTraySingleton::Instance();
    QObject::connect(systemTray, &DanmakuTray::toggleUDPServerStatus, &danmaku, &DanmakuUDPServer::toggleUDPServer);

    //internet subscribe thread
    this->subscribeThread = new SubscribeThread(this->_server,
                                                this->_channel,
                                                this->_password);
    QObject::connect(systemTray, &DanmakuTray::toggleUDPServerStatus,
                     this->subscribeThread, &SubscribeThread::switchPullStatus, Qt::QueuedConnection);
    this->subscribeThread->start();

    //danmaku enqueue connections
    connect(&this->danmaku, &DanmakuUDPServer::receivedDanmaku, this, &DanmakuEngine::enqueueDanmaku);
    connect(this->subscribeThread, &SubscribeThread::receivedDamanku, this, &DanmakuEngine::enqueueDanmaku, Qt::QueuedConnection);

    //danmakuTimer
    this->danmakuTimer = new QTimer();
    this->danmakuTimer->setInterval(50);
    QObject::connect(this->danmakuTimer,&QTimer::timeout, this, &DanmakuEngine::fireDanmaku,Qt::DirectConnection);
    this->danmakuTimer->start();
}

DanmakuEngine::~DanmakuEngine()
{
    danmakuQueue?(danmakuQueue->clear(), delete danmakuQueue,danmakuQueue=NULL):danmakuQueue=NULL;
    danmakuTimer?(danmakuTimer->stop(), delete danmakuTimer,danmakuTimer=NULL):danmakuTimer=NULL;
    if(settings){
        delete settings;
    }
}

void DanmakuEngine::start(bool yes)
{
    this->danmaku.toggleUDPServer(yes);
    this->subscribeThread->switchPullStatus(yes);
}

void DanmakuEngine::fireDanmaku()
{
    if(this->danmakuQueue&&!danmakuQueue->isEmpty()){
        this->mutex.lock();
        Danmaku_Msg dmku = danmakuQueue->dequeue();
        this->mutex.unlock();
        if(dmku.msg.isEmpty()){
            return;
        }
        BulletWidget *bw= NULL;
        while(bw==NULL){
            bw = new BulletWidget(dmku);
            bw->moveToThread(qApp->thread());
        }
    }
    qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
}

void DanmakuEngine::enqueueDanmaku(Danmaku_Msg dmku)
{
    if(this->danmakuQueue){
        this->mutex.lock();
        this->danmakuQueue->enqueue(dmku);
        this->mutex.unlock();
    }

}

void DanmakuEngine::saveServerSetting(QString server, QString channel, QString password)
{
    this->_server = server;
    this->_channel = channel;
    this->_password = password;
    if(this->subscribeThread)
    {
        this->subscribeThread->setServer(server);
        this->subscribeThread->setChannel(channel);
        this->subscribeThread->setPassword(password);
    }
}

QSettings *DanmakuEngine::getSettings()
{
    return this->settings;
}
