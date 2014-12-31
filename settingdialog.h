#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H
#include <QtCore>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QDialog>
#else
#include <QtGui/QDialog>
#endif
namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = 0);
    ~SettingDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::SettingDialog *ui;
private slots:
    void saveSettings();
    void cancelSettings();
    void createChannel();
    void enterChannel();
};

#endif // SETTINGDIALOG_H
