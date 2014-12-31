#ifndef WATERMARKWINDOW_H
#define WATERMARKWINDOW_H

#include <QWidget>

class WaterMarkWindow : public QWidget
{
    Q_OBJECT

public:
    WaterMarkWindow(QWidget *parent = 0);
    ~WaterMarkWindow();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void paintEvent(QPaintEvent *event);
private:
    QPoint windowPos, dPos;
    QString *waterMarkText;
    QImage  *waterMarkImage;
};

#endif // WATERMARKWINDOW_H
