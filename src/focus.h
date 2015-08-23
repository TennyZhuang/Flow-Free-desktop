#ifndef FOCUS_H
#define FOCUS_H
#include "common.h"
#include <QPoint>

class Focus
{
public:
    Focus();

private:
    QPoint point;
    Color color;
};

#endif // FOCUS_H
