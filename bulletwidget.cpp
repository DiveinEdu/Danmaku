#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QScreen>
#include <QDebug>
#include <QThread>
#include <QTime>
#ifdef Q_OS_LINUX
#include <QX11Info>
#include <X11/extensions/shape.h>
#endif
#ifdef Q_OS_WIN32
#include <windows.h>
#include <time.h>
#endif
#include "bulletwidget.h"

BulletWidget::BulletWidget(QWidget *parent) :
    QWidget(parent)
{

    this->init();
}

BulletWidget::BulletWidget(Danmaku_Msg &msg) :
    bulletText(msg.msg),
    bulletColor(msg.color),
    bulletPosition(msg.positon)
{
    this->setFont(QFont("宋体", 36));
    QFontMetrics fontMetrics = QFontMetrics(this->font());
    this->setFixedSize(fontMetrics.boundingRect(this->bulletText).size());
    this->init();
}

BulletWidget::BulletWidget(QString text, QColor textColor, BulletPosition postion) :
    bulletText(text),
    bulletColor(textColor),
    bulletPosition(postion)
{
    this->hide();
    this->setFont(QFont("宋体", 36));
    QFontMetrics fontMetrics = QFontMetrics(this->font());
    this->move(-1000,-1000);
    this->resize(fontMetrics.boundingRect(this->bulletText).size());
    this->init();

}

BulletWidget::~BulletWidget()
{
    delete this->animation;
}

void BulletWidget::paintEvent(QPaintEvent *event)
{

    Q_UNUSED(event)
    QPainter painter(this);
    if(!painter.isActive())
    {
        painter.begin(this);
    }
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.setRenderHints(QPainter::TextAntialiasing|QPainter::Antialiasing);


    QFont danmakuFont(this->font());
    painter.setFont(danmakuFont);


    QRect rect_l = this->rect();
    painter.fillRect(rect_l, Qt::transparent);


    painter.setPen(this->bulletColor);
    painter.drawText(rect_l,Qt::AlignLeft|Qt::AlignVCenter, this->bulletText);

    painter.end();
}


void BulletWidget::init()
{
    QFlags<Qt::WindowType>  flags;
    flags = Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint;
#ifdef Q_OS_LINUX
    flags |= Qt::SplashScreen;
#endif
    this->setWindowFlags(flags);

    this->setAttribute(Qt::WA_TranslucentBackground,true);
    this->setAttribute(Qt::WA_TransparentForMouseEvents,true);
    this->setAttribute(Qt::WA_MouseTracking,false);
    this->setWindowOpacity(1);
#ifdef Q_OS_LINUX
        XShapeCombineRectangles(QX11Info::display(), winId(), ShapeInput, 0,
                                0, NULL, 0, ShapeSet, YXBanded);
#endif
#ifdef Q_OS_WIN
        SetWindowLong((HWND)winId(), GWL_EXSTYLE, GetWindowLong((HWND)winId(), GWL_EXSTYLE) |
                      WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT | WS_EX_LAYERED);
        SetWindowPos((HWND)winId(),HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
#endif


        QSize screenSize = QApplication::screens().first()->geometry().size();
        int xPos = 10,yPos = 0;
        qsrand(QTime(0,0,0).secsTo(QTime::currentTime())*1000*1000);
        unsigned random = qrand();

        switch (this->bulletPosition) {
        case BULLET_TOP:
        case BULLET_BOTTOM:
            xPos += random%(screenSize.width()-this->size().width());
            if(this->bulletPosition == BULLET_BOTTOM){
                this->move(xPos, screenSize.height()-100);
            }else{
                this->move(xPos, 50);
            }
            animation = new QPropertyAnimation(this,"windowOpacity");
            animation->setStartValue(0.5f);
            animation->setKeyValueAt(0.3,1.0f);
            animation->setKeyValueAt(0.7,0.5f);
            animation->setEndValue(0.0f);
            animation->setDuration(1000*8);
            break;
        case BULLET_FLY:
        default:
            yPos = random%(screenSize.height()-100);
            this->move( (screenSize.width()), yPos);

            animation = new QPropertyAnimation(this, "pos");
            animation->setStartValue(this->pos());
            animation->setEndValue(QPoint(-this->width(),this->pos().ry()));
            animation->setDuration(1000*12);
            break;
        }
        connect(animation, &QPropertyAnimation::finished, this, &BulletWidget::deleteLater);
        this->setVisible(true);
        animation->start();

}
