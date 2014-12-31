#include <QUuid>
#include <QApplication>
#include <QDebug>
#include "subscribethread.h"
#include "bulletwidget.h"
SubscribeThread::SubscribeThread(QObject *parent) :
    QThread(parent),
    _needToPull(false)
{
    this->networkAccessManager = new QNetworkAccessManager();
}

SubscribeThread::SubscribeThread(QString server, QString channel, QString password) :
    _server(server.trimmed()), _channel(channel.trimmed()),
    _password(password.trimmed()), _uuid(QUuid::createUuid().toString()),
    _needToPull(false)
{
    this->_uuid.chop(1);
    this->_uuid.remove(0,1);
    this->networkAccessManager = new QNetworkAccessManager();
}

SubscribeThread::~SubscribeThread()
{
    delete this->subscribeTimer;
    delete this->networkAccessManager;
}

void SubscribeThread::setServer(QString &server)
{
    this->_server = server;
}

void SubscribeThread::setChannel(QString &channel)
{
    this->_channel = channel;
}

void SubscribeThread::setPassword(QString &password)
{
    this->_password = password;
}

void SubscribeThread::run()
{


#if 0
    this->subscribeTimer = new QTimer();
    this->subscribeTimer->setInterval(2000);
    connect(this->subscribeTimer, SIGNAL(timeout()), this, SLOT(pullDanmaku()));
    this->subscribeTimer->start();
#else
//    this->pullDanmaku();
#endif

    this->exec();

}


void SubscribeThread::subscribeDanmakuChannel(QString &channel)
{
    QString uri = QString("/api/v1/channels/%1/danmaku").arg(!channel.isEmpty()?channel:"demo");

    if(this->_server.endsWith("/") && uri.startsWith("/"))
    {
        this->_server.chop(1);
    }
    QString url = this->_server + uri;
    this->subscribeDanmaku(url);
    qDebug()<<"subscribeDanmaku:"<<url;
}
void SubscribeThread::subscribeDanmaku(QString &url)
{
    QNetworkRequest request;
    request.setUrl(QUrl(url));
//    qDebug()<<"uuid:"<<this->_uuid<<"key:"<<this->_password;
    request.setRawHeader("User-Agent", "DiveineduDanmaku");
    request.setRawHeader("X-GDANMAKU-SUBSCRIBER-ID", this->_uuid.toLocal8Bit());
    request.setRawHeader("X-GDANMAKU-AUTH-KEY", this->_password.toLocal8Bit());


    QNetworkReply *reply = networkAccessManager->get(request);

    connect(reply, SIGNAL(finished()), this, SLOT(processSubscribeDanmakuResponse()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(networkErrorOccured(QNetworkReply::NetworkError)));


}

void SubscribeThread::switchPullStatus(bool on)
{
    if(on){
        this->pullDanmaku();
    }
    this->_needToPull = on;
    qDebug()<<"Turn online danmaku subscribe:"<<(on?"on":"off");
}

void SubscribeThread::processSubscribeDanmakuResponse()
{

    QNetworkReply *reply = (QNetworkReply *)sender();
    int statusCode = reply->attribute( QNetworkRequest::HttpStatusCodeAttribute ).toInt();
    if(reply->hasRawHeader("Location")){
        QString strLocation = reply->header(QNetworkRequest::LocationHeader).toString();

        this->subscribeDanmaku(strLocation);

    }else if(statusCode == 200){

        QByteArray responseData = reply->readAll();
//        qDebug()<<"response data:"<<responseData;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        if(jsonDoc.isNull() ||!jsonDoc.isArray())
        {//  null or not array format
            goto out;
        }
        QJsonArray jsonArray = jsonDoc.array();
        for(int i = 0; i < jsonArray.count();i++){
            QJsonValue jsonValue = jsonArray[i];
            if(jsonValue.isUndefined()||jsonValue.isNull())
            {
                goto out;
            }
            QString text = jsonValue.toObject().value("text").toString();
            QString position = jsonValue.toObject().value("position").toString();
            QString style = jsonValue.toObject().value("style").toString();


            if(text.size()>128){
                text.resize(128);
            }
            Danmaku_Msg msg;
            msg.msg = text;
            if(style=="blue"){
                msg.color = QColor("#0000FF");
            }else if(style=="white"){
                msg.color = QColor("#FFFFFF");
            }else if(style=="red"){
                msg.color = QColor("#FF0000");
            }else if(style=="yellow"){
                msg.color = QColor("yellow");
            }else if(style=="cyan"){
                msg.color = QColor("cyan");
            }else if(style=="green"){
                msg.color = QColor("#00FF00");
            }else if(style=="purple"){
                msg.color = QColor("#871F78");
            }else{
                msg.color = QColor("black");
            }



            if(position=="top"){
                msg.positon = BULLET_TOP;
            }else if(position == "bottom")
            {
                msg.positon = BULLET_BOTTOM;
            }else
            {
                msg.positon = BULLET_FLY;
            }

            emit this->receivedDamanku(msg);
        }
    }
out:

    reply->deleteLater();
    qApp->processEvents(QEventLoop::AllEvents);
    this->pullDanmaku();
    return;
}

void SubscribeThread::networkErrorOccured(QNetworkReply::NetworkError code)
{
    Q_UNUSED(code);
    QNetworkReply *reply = (QNetworkReply *)sender();
    qDebug()<<Q_FUNC_INFO<<reply->errorString();

    reply->deleteLater();
    qApp->processEvents(QEventLoop::AllEvents);

}

void SubscribeThread::pullDanmaku()
{
    if(this->isRunning() == false){
        return;
    }
    if(this->_needToPull){
        this->subscribeDanmakuChannel(this->_channel);

    }else{
        QTimer::singleShot(10,this,SLOT(pullDanmaku()));
    }
}
