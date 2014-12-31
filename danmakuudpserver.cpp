#include "danmakuudpserver.h"
#include "bulletwidget.h"

DanmakuUDPServer::DanmakuUDPServer(QObject *parent) :
    QObject(parent),
    broadCastTimer(NULL),
    started(false)
{


}

DanmakuUDPServer::~DanmakuUDPServer()
{

    broadCastTimer?(broadCastTimer->stop(), delete broadCastTimer,broadCastTimer=NULL):broadCastTimer=NULL;

}

void DanmakuUDPServer::startUDPServer()
{
    if(!started){
        this->udpServerSocket = new QUdpSocket(this);
        this->udpServerSocket->bind(QHostAddress::Any, 65535);
        connect(this->udpServerSocket, &QUdpSocket::readyRead, this, &DanmakuUDPServer::processPendingDatagrams);

        this->broadCastTimer = new QTimer();
        this->broadCastTimer->setInterval(1000);
        connect(this->broadCastTimer,&QTimer::timeout, this, &DanmakuUDPServer::broadCast);
        this->broadCastTimer->start();



        started=true;
    }
}

void DanmakuUDPServer::stopUDPServer()
{
    if(started){
        disconnect(this->broadCastTimer,&QTimer::timeout, this, &DanmakuUDPServer::broadCast);
        this->broadCastTimer->stop();
        this->broadCastTimer->deleteLater();

        disconnect(this->udpServerSocket, &QUdpSocket::readyRead, this, &DanmakuUDPServer::processPendingDatagrams);
        this->udpServerSocket->deleteLater();

        started=false;
    }
}

void DanmakuUDPServer::toggleUDPServer(bool start)
{
    start?this->startUDPServer():this->stopUDPServer();
}

void DanmakuUDPServer::broadCast()
{
    QByteArray data("Damaku UDP Server");
    this->udpServerSocket->writeDatagram(data, QHostAddress::Broadcast,65534);
}



void DanmakuUDPServer::processPendingDatagrams()
{
    while (this->udpServerSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(this->udpServerSocket->pendingDatagramSize());
        this->udpServerSocket->readDatagram(datagram.data(), datagram.size());
        Danmaku_Msg msg;
        msg.positon = BULLET_FLY;
#ifdef Q_OS_OSX
        unsigned random = arc4random();
        msg.positon = (BulletPosition)(random%((unsigned)BULLET_INVALID));
#endif
        msg.msg = QString(datagram).trimmed();
        if(msg.msg.size()>1024){
            msg.msg.resize(1024);
        }
        if(!msg.msg.isEmpty()){
            msg.color = QColor("#dd4814");
            emit this->receivedDanmaku(msg);
        }

    }
}
