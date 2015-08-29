#ifndef POINT_H
#define POINT_H
#include <QObject>
#include "common.h"

struct GamePoint {
    quint32 col;
    quint32 row;
    Color color;
    bool isEndpoint;

    GamePoint();
    GamePoint(quint32 x, quint32 y, Color color): col(x), row(y), color(color), isEndpoint((bool)color) { }
    bool operator == (const GamePoint& point2) const;
    GamePoint operator = (const GamePoint &point);
};

#endif // POINT_H
