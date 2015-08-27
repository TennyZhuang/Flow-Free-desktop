#ifndef COMMON
#define COMMON
#include <QObject>

enum class Color
{
    NONE = 0,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    ORANGE,
    CYAN,
    PINK
};

const qint32 SCEAN_SIZE = 400;
const qint32 SCEAN_PADDING = 10;
const qint32 EPS = 2;

#endif // COMMON

