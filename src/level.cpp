#include "level.h"

Level::Level() {

}

int Level::getSize() const {
    return size;
}

int Level::getId() const {
    return id;
}

const std::vector<std::vector<GamePoint>>& Level::getPoints() const {
    return points;
}
quint32 Level::getColorsSize() const {
    return colorsSize;
}

