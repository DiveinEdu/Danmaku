#ifndef DANMAKUTRAY_H
#define DANMAKUTRAY_H
#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include "settingdialog.h"
#include "singleton.h"
class DanmakuTray : public QWidget
{
    Q_OBJECT
public:
    explicit DanmakuTray(QWidget *parent = 0);
    ~DanmakuTray();

signals:
    void toggleUDPServerStatus(bool start);
public slots:

private slots:
    void handleUDPServerAction(bool start);
    void settingActionTriggered();
    void quitActionTriggered();
private:
    QSystemTrayIcon systemTrayIcon;
    QMenu *menu;
    QAction *switchAction,*settingAction,*quitAction;
    SettingDialog *settingDialog;
};

typedef Singleton<DanmakuTray> DanmakuTraySingleton;   // Global declaration
#endif // DANMAKUTRAY_H
