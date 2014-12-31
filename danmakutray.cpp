#include <QDebug>
#include <QApplication>
#include "danmakutray.h"

DanmakuTray::DanmakuTray(QWidget *parent) :
    QWidget(parent),
    settingDialog(NULL)
{
    systemTrayIcon.setIcon(QIcon(":/image/danmaku-icon.png"));
    systemTrayIcon.show();
    menu = new QMenu(this);
    systemTrayIcon.setContextMenu(menu);

    //start stop action
    switchAction = menu->addAction(QObject::tr("&Start"));
    switchAction->setCheckable(true);
    switchAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_T));
    connect(this->switchAction,&QAction::toggled, this, &DanmakuTray::handleUDPServerAction);

    //setting action

    settingAction = menu->addAction(QObject::tr("&Option"));
    settingAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    connect(this->settingAction, &QAction::triggered, this, &DanmakuTray::settingActionTriggered);

    //quit action
    quitAction = menu->addAction(QObject::tr("&Quit"));
    quitAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
    connect(this->quitAction, &QAction::triggered, this, &DanmakuTray::quitActionTriggered);
}

DanmakuTray::~DanmakuTray()
{

    delete this->menu;
}

void DanmakuTray::handleUDPServerAction(bool start)
{
    this->switchAction->setText(start?tr("&Stop"):tr("&Start"));
    emit this->toggleUDPServerStatus(start);
}

void DanmakuTray::settingActionTriggered()
{

    if(this->settingDialog == NULL){
        this->settingDialog = new SettingDialog();
    }

    this->settingDialog->show();
    this->settingDialog->raise();
}

void DanmakuTray::quitActionTriggered()
{
    qDebug()<<Q_FUNC_INFO;
    exit(0);
}
