#include "common.h"
#include "gamescene.h"
#include "completedialog.h"
#include <QPainter>
#include <QPen>
#include <QDebug>
#include <QColor>
#include <QMouseEvent>
#include <QDialog>
#include <QMessageBox>
#include <iostream>
#include <assert.h>

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
    case Color::CYAN:
        return Qt::cyan;
        break;
    case Color::PINK:
        return QColor(240, 98, 146);
        break;
    default:
        assert(false);
        return Qt::transparent;
    }
}

inline bool overBound(int x, int y) {
    return (x >= SCEAN_SIZE || x < 0 ||
            y >= SCEAN_SIZE || y < 0);
}

}

GameScene::GameScene(QWidget *parent) : QWidget(parent)
{
    connectedSound = new QSound(":/sounds/connect.wav");
    breakedSound = new QSound(":/sounds/break.wav");
}

GameScene::~GameScene()
{
    delete connectedSound;
    delete breakedSound;
    if (currentPoint)
        delete currentPoint;
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
        p.drawLine(SCEAN_PADDING + 2,
                   SCEAN_PADDING + 2 + i * spacing,
                   SCEAN_SIZE - SCEAN_PADDING,
                   SCEAN_PADDING + 2 + i * spacing);

        p.drawLine(SCEAN_PADDING + 2 + i * spacing,
                   SCEAN_PADDING + 2,
                   SCEAN_PADDING + 2 +  i * spacing,
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

    setCursor(Qt::OpenHandCursor);
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

    if (tempRow == currentPoint->row && tempCol == currentPoint->col)
    {
        // point not changed
        repaint();
        return;
    }

    // overstep
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
        if (currentPoint->isEndpoint && tempPoint->color != currentColor)
        {
            repaint();
            return;
        }
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
            if (tempPoint->color != currentColor)
                breakedSound->play();
            routes[(int)tempPoint->color].eraseAfter(tempPoint);
        }
    }

    focus->isActive = true;
    if (routes[(int)currentColor].getEndpoints() == 2)
    {
        repaint();
        return;
    }

    if (!(*((routes[(int)currentColor]).getPoints().end() - 1) == tempPoint))
    {
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
    if (routes[(int)currentColor].getEndpoints() == 2)
        connectedSound->play();
    focus = nullptr;
    setCursor(Qt::ArrowCursor);
    currentColor = Color::NONE;
    currentPoint = nullptr;
    int pointsCnt = 0;
    int routesCnt = 0;
    for (const auto& route: routes)
    {
        if (route.getEndpoints() == 2)
            ++routesCnt;
        pointsCnt += route.getLength();
        qDebug() << routesCnt << pointsCnt;
    }

    if (routesCnt == colorsSize)
    {
        complete(pointsCnt);
    }

    repaint();
}

void GameScene::complete(int pointsCount)
{
//    qDebug() << "complete";
    if (pointsCount == colorsSize * colorsSize)
    {
        CompleteDialog dialog;
        if (dialog.exec())
        {
            emit nextLevel();
        }
    }
    else
    {
        QMessageBox::about(nullptr, "alert", "You should <b>fill</b> the puzzle with pipes.");
    }
}

inline quint32 GameScene::convertIndexToGridCenterPixel(quint32 index) const
{
    return SCEAN_PADDING + 2 + (index + 0.5) * spacing;
}

inline quint32 GameScene::convertIndexToPixel(quint32 index) const
{
    return SCEAN_PADDING + 2 + index * spacing;
}

inline quint32 GameScene::convertPixelToIndex(quint32 pixel) const
{
    if (pixel < SCEAN_PADDING)
        return 0;
    if (pixel >= SCEAN_SIZE - SCEAN_PADDING)
        return gameSize - 1;
    return (pixel - SCEAN_PADDING) / spacing;
}

void GameScene::onLoadLevel(quint32 currentLevelId)
{
    focus = nullptr;
    auto gameModal = GameModel::instance();
    assert(currentLevelId > 0 || currentLevelId <= gameModal->size());

    const Level level = gameModal->getLevel(currentLevelId);
    points = level.getPoints();
    gameSize = level.getSize();
    colorsSize = level.getColorsSize();
    spacing = (SCEAN_SIZE - SCEAN_PADDING * 2) / gameSize;
    diameter = spacing * 0.8;
    this->currentLevelId = currentLevelId;
    routes.clear();
    routes.resize(colorsSize + 1, points);
    for (int i = 1; i <= colorsSize; i++)
        routes[i].setColor((Color)i);
    currentPoint = nullptr;
    repaint();
}

void GameScene::autoSolve()
{
    qDebug() << "solving";
}
