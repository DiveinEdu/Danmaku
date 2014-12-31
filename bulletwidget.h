#ifndef BULLETWIDGET_H
#define BULLETWIDGET_H

#include <QWidget>
#include <QPropertyAnimation>
#include "defines.h"

class BulletWidget : public QWidget
{
    Q_OBJECT

public:
    BulletWidget(Danmaku_Msg &msg);
    BulletWidget(QString text, QColor textColor, BulletPosition postion=BULLET_FLY);
    ~BulletWidget();
    void paintEvent(QPaintEvent *event);

signals:

public slots:
private:
    QString bulletText;
    QColor bulletColor;
    BulletPosition bulletPosition;
    QPropertyAnimation *animation;


    //private methods
    explicit BulletWidget(QWidget *parent = 0);
    void init();
private slots:

};

#endif // BULLETWIDGET_H
