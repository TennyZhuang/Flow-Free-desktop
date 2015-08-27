#include "gameroute.h"
#include <QDebug>

GameRoute::GameRoute(const std::vector<std::vector<GamePoint> > &points):
    points(points),
    endpoints(0)
{
    p1 = nullptr;
    p2 = nullptr;
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

void GameRoute::popPoint()
{
    if (routePoints.back()->isEndpoint)
        --endpoints;
    if (routePoints.size())
        routePoints.pop_back();
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
    for (int i = 0; i < points.size(); i++)
    {
        for (int j = 0; j < points.size(); j++)
        {
            if (points[j][i].color == value)
            {
                if (p1)
                {
                    p2 = &points[j][i];
                    return;
                }
                else
                {
                    p1 = &points[j][i];
                }
            }
        }
    }
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

GamePoint *GameRoute::getP1() const
{
    return p1;
}

GamePoint *GameRoute::getP2() const
{
    return p2;
}




