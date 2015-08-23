#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include "level.h"
#include <vector>

class Level;

class GameModel
{
public:
    ~GameModel();

    static GameModel* instance();

    // Level Controller

    quint32 size() const;

    // getter
    const Level& getLevel(quint32 levelId) const;
private:
    // Singleton instance
    static GameModel* _instance;

    GameModel();
    // Singleton. Prevent default copy
    GameModel(const GameModel&) = delete;
    GameModel(GameModel&&) = delete;
    GameModel& operator = (const GameModel&) = delete;

    // initialize levels data
    bool read();

    // levels data
    std::vector<Level> levels;

    // level size
    quint32 levelSize;

    // current level ID
    quint32 currentLevelId;
};

#endif // GAMEMODEL_H
