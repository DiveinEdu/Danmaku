#include <QSettings>
#include <QMessageBox>
#include <QDesktopServices>
#include "settingdialog.h"
#include "ui_settingdialog.h"
#include "danmakuengine.h"
SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveSettings()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancelSettings()));
    connect(ui->createChannelButton, SIGNAL(clicked()), this, SLOT(createChannel()));
    connect(ui->enterChannelButton, SIGNAL(clicked()), this, SLOT(enterChannel()));

    QSettings *settings = DanmakuEngineSingleton::Instance()->getSettings();
    if(settings->value("firstrun").toString()=="false"){
        ui->serverLineEdit->setText(settings->value("server").toString());
        ui->channelLineEdit->setText(settings->value("channel").toString());
        ui->passwordLineEdit->setText(settings->value("password").toString());
    }else
    {
        settings->setValue("firstrun","false");
        settings->sync();
    }
    this->raise();
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

void SettingDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void SettingDialog::saveSettings()
{

    if(this->ui->serverLineEdit->text().isEmpty()
            ||this->ui->channelLineEdit->text().isEmpty()){
        QMessageBox::warning(this,tr("警告"),tr("信息不完整(密码可为空)"),tr("确定"));
        return;
    }
    DanmakuEngine *engine = DanmakuEngineSingleton::Instance();
    engine->saveServerSetting(ui->serverLineEdit->text(),
                              ui->channelLineEdit->text(),
                              ui->passwordLineEdit->text());

    QSettings *settings = engine->getSettings();
    settings->setValue("server", this->ui->serverLineEdit->text());
    settings->setValue("channel", this->ui->channelLineEdit->text());
    settings->setValue("password", this->ui->passwordLineEdit->text());
    settings->sync();
    this->hide();

    return;
}

void SettingDialog::cancelSettings()
{
    this->hide();

}

void SettingDialog::createChannel()
{
    QUrl url(DanmakuEngineSingleton::Instance()->getSettings()->value("server").toString());
    if(ui->serverLineEdit->text().isEmpty() == false){
        url.setUrl(ui->serverLineEdit->text());
    }
    QDesktopServices::openUrl(url);
}

void SettingDialog::enterChannel()
{
    QString defaultserver = DanmakuEngineSingleton::Instance()->getSettings()->value("server").toString();
    QString defaultchannel = DanmakuEngineSingleton::Instance()->getSettings()->value("channel").toString();
    QString urlStr = defaultserver +"/channel/"+defaultchannel;
    QUrl url(urlStr);
    if(ui->serverLineEdit->text().isEmpty() == false&&ui->channelLineEdit->text().isEmpty()==false){
        url.setUrl(ui->serverLineEdit->text()+"/channel/"+ui->channelLineEdit->text());
    }
    QDesktopServices::openUrl(url);
}
