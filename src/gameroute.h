#ifndef GAMEROUTE_H
#define GAMEROUTE_H
#include "gamepoint.h"
#include <memory>

enum class Direction
{
    HORIZON = 0,
    VERTICAL
};

class GameRoute
{
public:
    GameRoute(const std::vector<std::vector<GamePoint>>& points);
    void eraseAfter(GamePoint* point, bool plus = false);
    int getLength() const;
    void addPoint(GamePoint* point);
    void clear();

    const std::vector<GamePoint*> &getPoints() const;

    Color getColor() const;

    void setColor(const Color &value);

    quint32 getEndpoints() const;

    void addEndpoint(GamePoint* point);

private:
    quint32 endpoints;
    Color color;
    std::vector<std::vector<GamePoint>> points;
    std::vector<GamePoint*> routePoints;
};

#endif // GAMEROUTE_H
