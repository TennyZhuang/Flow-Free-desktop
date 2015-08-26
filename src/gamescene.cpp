#include "common.h"
#include "gamescene.h"
#include <QPainter>
#include <QPen>
#include <QDebug>
#include <QColor>
#include <QMouseEvent>
#include <iostream>

namespace
{

QColor convertToQColor(Color color)
{
    switch(color)
    {
    case Color::RED:
        return Qt::red;
        break;
    case Color::GREEN:
        return Qt::green;
        break;
    case Color::YELLOW:
        return Qt::yellow;
        break;
    case Color::BLUE:
        return Qt::blue;
        break;
    case Color::ORANGE:
        return QColor(250, 128, 10);
        break;
    }
}

inline bool overBound(int x, int y) {
    return (x >= SCEAN_SIZE || x < 0 ||
            y >= SCEAN_SIZE || y < 0);
}

}

GameScene::GameScene(QWidget *parent) : QWidget(parent)
{
    loadLevel(1);
}

bool GameScene::loadLastLevel()
{
    return loadLevel(currentLevelId - 1);
}

bool GameScene::loadNextLevel()
{
    return loadLevel(currentLevelId + 1);
}

void GameScene::clearRoutes()
{
    loadLevel(currentLevelId);
}

bool GameScene::loadLevel(quint32 levelId)
{
    focus = nullptr;
    auto gameModal = GameModel::instance();
    if (levelId <= 0 || levelId > gameModal->size())
        return false;

    const Level level = gameModal->getLevel(levelId);
    points = level.getPoints();
    gameSize = level.getSize();
    colorsSize = level.getColorsSize();
    spacing = (SCEAN_SIZE - SCEAN_PADDING * 2) / gameSize;
    diameter = spacing * 0.8;
    currentLevelId = levelId;
    routes.clear();
    routes.resize(colorsSize + 1, points);
    for (int i = 1; i <= colorsSize; i++)
        routes[i].setColor((Color)i);
    currentPoint = nullptr;
    qDebug() << " " << routes.size();
    repaint();
    return true;
}

void GameScene::paintEvent(QPaintEvent *ev)
{
    // draw background
    QPainter p(this);
    p.fillRect(0, 0, SCEAN_SIZE, SCEAN_SIZE, Qt::black);



    // draw grids
    p.setPen(QPen(QColor(100, 255, 218), 2));

    for (quint32 i = 0; i <= gameSize; i++)
    {
        p.drawLine(SCEAN_PADDING + 1,
                   SCEAN_PADDING + 1 + i * spacing,
                   SCEAN_SIZE - SCEAN_PADDING,
                   SCEAN_PADDING + 1 + i * spacing);

        p.drawLine(SCEAN_PADDING + 1 + i * spacing,
                   SCEAN_PADDING + 1,
                   SCEAN_PADDING + 1 +  i * spacing,
                   SCEAN_SIZE - SCEAN_PADDING);
    }

    // draw routes
    for (int i = 1; i <= colorsSize; i++)
    {
        const auto& points = routes[i].getPoints();

        if (points.size() < 1)
            continue;


        QPen pen(convertToQColor(routes[i].getColor()), 20);
        pen.setJoinStyle(Qt::RoundJoin);
        p.setPen(pen);
        for (int j = 0; j < points.size() - 1; j++)
        {
            p.drawLine(convertIndexToGridCenterPixel(points[j]->col),
                       convertIndexToGridCenterPixel(points[j]->row),
                       convertIndexToGridCenterPixel(points[j + 1]->col),
                       convertIndexToGridCenterPixel(points[j + 1]->row));
        }

        p.setPen(QPen(Qt::transparent, 0));
        auto qColor = convertToQColor(routes[i].getColor());
        qColor.setAlphaF(0.4);

        if (i == (int)currentColor)
            continue;
        for (auto& point: points)
        {
            p.fillRect(convertIndexToPixel(point->col),
                       convertIndexToPixel(point->row),
                       spacing,
                       spacing,
                       qColor);
        }
    }

    // draw point
    p.setPen(QPen(Qt::transparent, 0));
    for (const auto& pointRow: points)
    {
        for (const auto& point: pointRow)
        {
            if ((bool)point.color && point.isEndpoint)
            {
                p.setBrush(QBrush(convertToQColor(point.color)));
                p.drawEllipse(QPoint(convertIndexToGridCenterPixel(point.col),
                                     convertIndexToGridCenterPixel(point.row)),
                              diameter >> 1, diameter >> 1);
            }
//            std::cout << (int)point.color << ' ';
        }
//        std::cout << std::endl;
    }
//    std::cout << std::endl;
//    std::cout << std::endl;
//    std::cout << std::endl;

    // draw foucus
    if (focus)
    {
        QColor qcolor = convertToQColor(focus->color);
        qcolor.setAlphaF((focus->isActive)? 0.6: 0.3);
        p.setBrush(qcolor);
        p.drawEllipse(*focus, diameter, diameter);
    }
}

void GameScene::mousePressEvent(QMouseEvent* ev)
{
    if (overBound(ev->x(), ev->y()))
        return;

    setCursor(Qt::CrossCursor);
    currentPoint = &points[convertPixelToIndex(ev->y())][convertPixelToIndex(ev->x())];
    if ((bool)currentPoint->color)
    {
        currentColor = currentPoint->color;
        focus = new FocusPoint(ev->x(), ev->y(),
                               currentPoint->color);
//        currentColor = currentPoint->color;
        if (currentPoint->isEndpoint)
        {
            routes[(int)currentColor].clear();
            routes[(int)currentColor].addEndpoint(currentPoint);
//            qDebug() << routes[(int)currentColor].getEndpoints();
        }
        else
        {
//            qDebug() << " " << currentPoint->col << currentPoint->row;
            routes[(int)currentColor].eraseAfter(currentPoint, true);
        }
    }

    repaint();
}

void GameScene::mouseMoveEvent(QMouseEvent *ev)
{
    // over screen
    if (overBound(ev->x(), ev->y()))
        return;

    // update focus
    if (focus)
    {
        focus->setX(ev->x());
        focus->setY(ev->y());
    }
    else
    {
        return;
    }

    // check point update
    int tempRow = convertPixelToIndex(ev->y());
    int tempCol = convertPixelToIndex(ev->x());

//    qDebug() << tempCol << tempRow;
//    qDebug() << " " << currentPoint->col << currentPoint->row;
    if (tempRow == currentPoint->row && tempCol == currentPoint->col)
    {
        // point not changed
        repaint();
        return;
    }

    // overstep
//    qDebug() << gameSize;
    if (abs(currentPoint->col - tempCol) + abs(currentPoint->row - tempRow) > 1)
    {
        focus->isActive = false;
        repaint();
        return;
    }

    GamePoint* tempPoint = &points[tempRow][tempCol];
    if ((bool)tempPoint->color)
    {
        // have color
        if (tempPoint->isEndpoint)
        {
            // is end point
            if (tempPoint->color == currentColor)
            {
                // same color
                if (tempPoint == routes[(int)currentColor].getPoints()[0])
                {
                    // start point
                    // restart
                    routes[(int)currentColor].clear();
                }
                else
                {
                    // end point
//                    qDebug() << (int)tempPoint->color;

                }
                routes[(int)currentColor].addEndpoint(tempPoint);
                currentPoint = tempPoint;
            }
            else
            {
                // differenet color
                // prevent default
                focus->isActive = false;
            }

            repaint();
            return;
        }
        else
        {
            // not end point
            // break current route
            routes[(int)tempPoint->color].eraseAfter(tempPoint);
        }
    }

    focus->isActive = true;
//    qDebug() << routes[(int)currentColor].getEndpoints();
    if (routes[(int)currentColor].getEndpoints() == 2)
    {
        repaint();
        return;
    }

    if (!(*((routes[(int)currentColor]).getPoints().end() - 1) == tempPoint))
    {
//        qDebug() << &points[tempRow][tempCol];
        routes[(int)currentColor].addPoint(&points[tempRow][tempCol]);
    }
    tempPoint->color = currentColor;
    currentPoint = &points[tempRow][tempCol];

    repaint();
}

void GameScene::mouseReleaseEvent(QMouseEvent *ev)
{
    if (focus)
        delete focus;
    focus = nullptr;
    setCursor(Qt::ArrowCursor);
    currentColor = Color::NONE;
    currentPoint = nullptr;
    repaint();
}

inline quint32 GameScene::convertIndexToGridCenterPixel(quint32 index) const
{
    return SCEAN_PADDING + (index + 0.5) * spacing;
}

inline quint32 GameScene::convertIndexToPixel(quint32 index) const
{
    return SCEAN_PADDING + index * spacing;
}

inline quint32 GameScene::convertPixelToIndex(quint32 pixel) const
{
    if (pixel < SCEAN_PADDING)
        return 0;
    if (pixel >= SCEAN_SIZE - SCEAN_PADDING)
        return gameSize - 1;
    return (pixel - SCEAN_PADDING) / spacing;
}
