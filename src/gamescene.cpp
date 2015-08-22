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
        return Qt::darkYellow;
        break;
    }
}

}

GameScene::GameScene(QWidget *parent) : QWidget(parent)
{
    auto instance = GameModel::instance();
    const Level level = instance->getLevel(1);
    points = level.getPoints();
    gameSize = level.getSize();
}

void GameScene::paintEvent(QPaintEvent *ev)
{
    // draw background
    QPainter p(this);
    p.fillRect(0, 0, 400, 400, Qt::black);

    // draw grids
    p.setPen(QPen(QColor(100, 255, 218), 2));
    quint32 spacing = (SCEAN_SIZE - SCEAN_PADDING * 2) / gameSize;
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
    quint32 diameter = (int)(spacing * 0.8);
    for (const auto& pointRow: points)
    {
        for (const auto& point: pointRow)
        {
            if ((bool)point.color)
            {
                p.setBrush(QBrush(convertToQColor(point.color)));
                p.drawEllipse(SCEAN_PADDING + (point.x + 0.1) * spacing, SCEAN_PADDING + (point.y + 0.1) * spacing, diameter, diameter);
            }
        }
    }
}

