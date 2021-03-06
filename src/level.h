#ifndef LEVEL_H
#define LEVEL_H

#include "common.h"
#include "gamepoint.h"
#include <vector>

class Level {
  public:
    Level();

    // Game Model can write level data
    friend class GameModel;

    // getter
    int getSize() const;

    int getId() const;

    const std::vector<std::vector<GamePoint>>& getPoints() const;

    quint32 getColorsSize() const;

  private:
    quint32 id;
    quint32 size;
    quint32 colorsSize;
    std::vector<std::vector<GamePoint>> points;
};

#endif // LEVEL_H
