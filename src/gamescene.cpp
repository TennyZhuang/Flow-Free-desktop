#include "common.h"
#include "gamescene.h"
#include <QPainter>
#include <QPen>
#include <QDebug>
#include <QColor>

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

bool GameScene::loadLevel(quint32 levelId)
{
    auto gameModal = GameModel::instance();
    if (levelId <= 0 || levelId > gameModal->size())
        return false;

    const Level level = gameModal->getLevel(levelId);
    points = level.getPoints();
    gameSize = level.getSize();
    spacing = (SCEAN_SIZE - SCEAN_PADDING * 2) / gameSize;
    diameter = spacing * 0.8;
    currentLevelId = levelId;
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
        p.drawLine(SCEAN_PADDING,
                   SCEAN_PADDING + i * spacing,
                   SCEAN_SIZE - SCEAN_PADDING,
                   SCEAN_PADDING + i * spacing);

        p.drawLine(SCEAN_PADDING + i * spacing,
                   SCEAN_PADDING,
                   SCEAN_PADDING + i * spacing,
                   SCEAN_SIZE - SCEAN_PADDING);
    }

    // draw point
    p.setPen(Qt::transparent);
    for (const auto& pointRow: points)
    {
        for (const auto& point: pointRow)
        {
            if ((bool)point.color)
            {
                p.setBrush(QBrush(convertToQColor(point.color)));
                p.drawEllipse(SCEAN_PADDING + (point.col + 0.1) * spacing, SCEAN_PADDING + (point.row + 0.1) * spacing, diameter, diameter);
            }
        }
    }

    // draw path
    // TODO

    // draw foucus
    // TODO

}

