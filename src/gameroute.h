#ifndef GAMEROUTE_H
#define GAMEROUTE_H
#include "gamepoint.h"

enum class Direction {
    HORIZON = 0,
    VERTICAL
};

class GameRoute {
  public:
    GameRoute(const std::vector<std::vector<GamePoint>>& points);
    void eraseAfter(GamePoint* point, bool plus = false);
    int getLength() const;
    void addPoint(GamePoint* point);
    void popPoint();
    void clear();

    const std::vector<GamePoint*> &getPoints() const;

    Color getColor() const;

    void setColor(const Color &value);

    quint32 getEndpoints() const;

    void addEndpoint(GamePoint* point);

    GamePoint *getP1() const;

    GamePoint *getP2() const;
    quint32 endpoints;
  private:
    Color color;
    GamePoint* p1;
    GamePoint* p2;
    std::vector<std::vector<GamePoint>> points;
    std::vector<GamePoint*> routePoints;
};

#endif // GAMEROUTE_H
