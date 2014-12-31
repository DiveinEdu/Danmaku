#ifndef SUBSCRIBETHREAD_H
#define SUBSCRIBETHREAD_H

#include <QThread>
#include <QtNetwork>
#include <QTimer>
#include "defines.h"
class SubscribeThread : public QThread
{
    Q_OBJECT
public:
    explicit SubscribeThread(QObject *parent = 0);
    SubscribeThread(QString server, QString channel, QString password);
    ~SubscribeThread();
    void setServer(QString &server);
    void setChannel(QString &channel);
    void setPassword(QString &password);
signals:
    void receivedDamanku(Danmaku_Msg msg);
public slots:
    void run();
    void subscribeDanmakuChannel(QString &channel);
    void subscribeDanmaku(QString &url);
    void switchPullStatus(bool on);
private slots:
    void processSubscribeDanmakuResponse();
    void networkErrorOccured(QNetworkReply::NetworkError code);
    void pullDanmaku();
private:
    QString _server, _channel, _password, _uuid;
    QTimer  *subscribeTimer;
    QNetworkAccessManager *networkAccessManager;
    bool _needToPull;
};

#endif // SUBSCRIBETHREAD_H
