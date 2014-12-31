#include <QObject>
#include <QApplication>
#include <QScreen>
#include <QDebug>
#include "watermarkwindow.h"
#include "bulletwidget.h"
#include "danmakuudpserver.h"
#include "danmakutray.h"
#include "subscribethread.h"
#include "danmakuengine.h"
int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("戴维营教育");
    QCoreApplication::setOrganizationDomain("diveinedu.com");
    QCoreApplication::setApplicationName("Danmaku");

    QApplication a(argc, argv);
    WaterMarkWindow w;
    w.show();
    DanmakuEngine *engine = DanmakuEngineSingleton::Instance();
    engine->start();
    QObject::connect(&a,SIGNAL(aboutToQuit()), engine, SLOT(deleteLater()));
    return a.exec();
}
