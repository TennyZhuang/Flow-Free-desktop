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
    void paintEvent(QPaintEvent *ev);

private:
    quint32 gameSize;
    quint32 colorSize;
    std::vector<std::vector<GamePoint>> points;
    std::vector<std::vector<Path>> paths;
signals:

public slots:
};

#endif // GAMESCENE_H
