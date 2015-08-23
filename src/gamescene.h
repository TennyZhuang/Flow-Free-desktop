#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "gamepoint.h"
#include "path.h"
#include "gamemodel.h"
#include <QWidget>

class GameScene : public QWidget
{
    Q_OBJECT
public:
    explicit GameScene(QWidget *parent = 0);
    bool loadLevel(quint32 levelId);
    void paintEvent(QPaintEvent *ev);

private:
    quint32 gameSize;
    quint32 colorSize;
    std::vector<std::vector<GamePoint>> points;
    std::vector<Path> paths;

    quint32 spacing;
    quint32 diameter;

    quint32 currentLevelId;
signals:

public slots:
    bool loadLastLevel();
    bool loadNextLevel();
};

#endif // GAMESCENE_H
