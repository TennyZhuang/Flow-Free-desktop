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

    auto pit = std::find(routePoints.begin(), routePoints.end(), point);
    pit = (plus)? pit + 1: pit;
    if (pit == routePoints.end())
    {
        return;
    }

    for_each(pit, routePoints.end(), [](GamePoint* point) {
        if (!point->isEndpoint)
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
    for_each(routePoints.begin(), routePoints.end(), [](GamePoint* point) {
        if (!point->isEndpoint)
            point->color = Color::NONE;
    });
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


