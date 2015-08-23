#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "gamepoint.h"
#include "gamemodel.h"
#include "gameroute.h"
#include <QWidget>

class GameScene : public QWidget
{
    Q_OBJECT
public:
    explicit GameScene(QWidget *parent = 0);
    bool loadLevel(quint32 levelId);
    void paintEvent(QPaintEvent *ev);
    void mousePressEvent(QMouseEvent* ev);
    void mouseMoveEvent(QMouseEvent* ev);
    void mouseReleaseEvent(QMouseEvent* ev);

    class FocusPoint: public QPoint
    {
    public:
        int x, y;
        Color color;
        FocusPoint() { }
        FocusPoint(int x, int y, Color color): QPoint(x, y), color(color) { }
    };

private:
    quint32 gameSize;
    quint32 colorSize;
    std::vector<std::vector<GamePoint>> points;
    std::vector<GameRoute> paths;
    FocusPoint* focus;

    quint32 spacing;
    quint32 diameter;

    quint32 currentLevelId;
    bool isClicked;
signals:

public slots:
    bool loadLastLevel();
    bool loadNextLevel();
};

#endif // GAMESCENE_H
