#ifndef DANMAKUUDPSERVER_H
#define DANMAKUUDPSERVER_H

#include <QObject>
#include <QUdpSocket>
#include <QTimer>

#include "defines.h"

class DanmakuUDPServer : public QObject
{
    Q_OBJECT
public:
    explicit DanmakuUDPServer(QObject *parent = 0);
    ~DanmakuUDPServer();
signals:
    void receivedDanmaku(Danmaku_Msg &msg);
public slots:
    void startUDPServer();
    void stopUDPServer();
    void toggleUDPServer(bool start);
    void broadCast();
    void processPendingDatagrams();
private:
    QUdpSocket *udpServerSocket;
    QTimer  *broadCastTimer;
    bool    started;

};

#endif // DANMAKUUDPSERVER_H
