#ifndef DANMAKUENGINE_H
#define DANMAKUENGINE_H

#include <QObject>
#include <QQueue>
#include <QTimer>
#include <QMutex>
#include <QSettings>
#include "danmakutray.h"
#include "danmakuudpserver.h"
#include "subscribethread.h"
#include "defines.h"
class DanmakuEngine : public QObject
{
    Q_OBJECT
public:
    explicit DanmakuEngine(QObject *parent = 0);
    ~DanmakuEngine();

signals:

public slots:
    void start(bool yes=false);
    void fireDanmaku();
    void enqueueDanmaku(Danmaku_Msg dmku);
    void saveServerSetting(QString server, QString channel, QString password);
    QSettings * getSettings();
private:
    DanmakuUDPServer danmaku;
    SubscribeThread *subscribeThread;
    QQueue<Danmaku_Msg>  *danmakuQueue;
    QTimer  *danmakuTimer;
    QMutex mutex;
    QString _server, _channel, _password;
    QSettings *settings;
};

typedef Singleton<DanmakuEngine> DanmakuEngineSingleton;   // Global declaration

#endif // DANMAKUENGINE_H
