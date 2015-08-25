#include "gameroute.h"
#include <QDebug>

GameRoute::GameRoute(const std::vector<std::vector<GamePoint> > &points):
    points(points),
    endpoints(0)
{

}

void GameRoute::eraseAfter(GamePoint *point, bool plus)
{
    qDebug() << "erasing";
    if (routePoints.size() < 1)
        return;
    if ((*(routePoints.end() - 1))->isEndpoint)
    {
        --endpoints;
    }
    else if ((*(routePoints.end() - 1)) != point)
    {
        (*(routePoints.end() - 1))->color = Color::NONE;
    }

    auto pit = std::find(routePoints.begin(), routePoints.end(), point);
    pit = (plus)? pit + 1: pit;
    if (pit == routePoints.end())
    {
        return;
    }

    for_each(pit, routePoints.end() - 1, [](GamePoint* point) {
        point->color = Color::NONE;
    });

    routePoints.erase(pit, routePoints.end());
}

int GameRoute::getLength() const
{
    return routePoints.size();
}

void GameRoute::addPoint(GamePoint *point)
{
    routePoints.push_back(point);
}

void GameRoute::clear()
{
    endpoints = 0;
    routePoints.clear();
}

const std::vector<GamePoint*>& GameRoute::getPoints() const
{
    return routePoints;
}

Color GameRoute::getColor() const
{
    return color;
}

void GameRoute::setColor(const Color &value)
{
    color = value;
}
quint32 GameRoute::getEndpoints() const
{
    return endpoints;
}

void GameRoute::addEndpoint(GamePoint *point)
{
    if (endpoints >= 2)
        return;
    addPoint(point);
    ++endpoints;
}


