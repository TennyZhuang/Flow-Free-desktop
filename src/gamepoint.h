#ifndef POINT_H
#define POINT_H
#include <QObject>
#include "common.h"

struct GamePoint {
    quint32 x;
    quint32 y;
    Color color;

    GamePoint();
    GamePoint(quint32 x, quint32 y, Color color): x(x), y(y), color(color) { }
};

#endif // POINT_H
