#ifndef DEFINES_H
#define DEFINES_H
#include <QColor>

typedef enum _bulletPosition{
    BULLET_FLY = 0,
    BULLET_TOP,
    BULLET_BOTTOM,
    BULLET_INVALID
}BulletPosition;

struct Danmaku_Msg{
    QString msg;
    QColor  color;
    BulletPosition positon;
};
#endif // DEFINES_H
