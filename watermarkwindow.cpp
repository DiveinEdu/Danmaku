#include <QMouseEvent>
#include <QPainter>
#include <QApplication>
#include <QScreen>
#include <QDebug>
#ifdef Q_OS_LINUX
#include <QX11Info>
#include <X11/extensions/shape.h>
#endif
#ifdef Q_OS_WIN32
#include <windows.h>
#endif
#include "watermarkwindow.h"

WaterMarkWindow::WaterMarkWindow(QWidget *parent)
    : QWidget(parent),
      waterMarkText(NULL),
      waterMarkImage(NULL)
{
    this->waterMarkText = new QString("戴维营教育iOS开发培训");
    this->waterMarkImage = new QImage(":/image/logo.png");


    this->setFixedSize(160,160);

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

    QSize screenSize = QApplication::screens().first()->geometry().size();
    this->move( (screenSize.width() - this->rect().width()), (screenSize.height() - this->rect().height()) - 100);
#ifdef Q_OS_LINUX
#if 0 // mouse tracking
        XShapeCombineRectangles(QX11Info::display(), winId(), ShapeInput, 0,
                                0, NULL, 0, ShapeSet, YXBanded);
#endif
#endif
#ifdef Q_OS_WIN
#if 0
        SetWindowLong((HWND)winId(), GWL_EXSTYLE, GetWindowLong((HWND)winId(), GWL_EXSTYLE) |
                      WS_EX_TRANSPARENT | WS_EX_LAYERED);
#endif
        SetWindowLong((HWND)winId(), GWL_EXSTYLE, GetWindowLong((HWND)winId(), GWL_EXSTYLE) |
                      WS_EX_TOOLWINDOW | WS_EX_LAYERED);
        SetWindowPos((HWND)winId(),HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
#endif
    this->window()->setWindowOpacity(0.75);
}

WaterMarkWindow::~WaterMarkWindow()
{
    delete waterMarkText;
    delete waterMarkImage;
}

void WaterMarkWindow::mousePressEvent(QMouseEvent *event)
{
    this->windowPos = this->pos();
    QPoint mousePos = event->globalPos();
    this->dPos = mousePos - windowPos;
    this->raise();

}

void WaterMarkWindow::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos() - this->dPos);
}

void WaterMarkWindow::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta() / 8;    //滚动的角度，*8就是鼠标滚动的距离
#define MAX_WIDTH   256
#define MIN_WIDTH   96
    if (event->orientation() == Qt::Horizontal) {
        int newWidth = this->size().width()+numDegrees;
        newWidth = newWidth<MAX_WIDTH?newWidth:MAX_WIDTH;
        newWidth = newWidth<MIN_WIDTH?MIN_WIDTH:newWidth;
        this->setFixedSize(newWidth,newWidth);  //水平滚动
    } else {
        qreal opacity = this->window()->windowOpacity() + numDegrees/360.0;
        qreal newOpacity = opacity > 0.1?opacity:0.1;
        this->window()->setWindowOpacity(newOpacity);   //垂直滚动
    }
    event->accept();    //接收该事件

}

void WaterMarkWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    if(!painter.isActive())
    {
        painter.begin(this);
    }
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.setRenderHints(QPainter::TextAntialiasing|QPainter::Antialiasing);

    if(this->waterMarkImage)
    {
        painter.drawImage(this->rect(), *this->waterMarkImage);
    }else{
        QFont font = QFont("宋体", 36);
        painter.setFont(font);

        QRect rect_l = this->rect();
        painter.fillRect(rect_l, Qt::transparent);


        painter.setPen(QColor("red"));
        painter.drawText(rect_l,Qt::AlignLeft|Qt::AlignVCenter, *this->waterMarkText);
    }
    painter.end();
}
