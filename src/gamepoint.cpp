#include "gamepoint.h"

GamePoint::GamePoint() {

}

bool GamePoint::operator ==(const GamePoint &point2) const {
    return (col == point2.col && row == point2.row);
}

GamePoint GamePoint::operator =(const GamePoint& point) {
    row = point.row;
    col = point.col;
    color = point.color;
    isEndpoint = point.isEndpoint;

    return *this;
}

