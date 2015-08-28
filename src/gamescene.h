#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "gamepoint.h"
#include "gamemodel.h"
#include "gameroute.h"
#include "completedialog.h"
#include <QWidget>
#include <QSound>

class CompleteDialog;
class QDialog;
class QLabel;

class GameScene : public QWidget {
    Q_OBJECT
  public:
    explicit GameScene(QWidget *parent = 0);
    ~GameScene();
    void paintEvent(QPaintEvent *ev);
    void mousePressEvent(QMouseEvent* ev);
    void mouseMoveEvent(QMouseEvent* ev);
    void mouseReleaseEvent(QMouseEvent* ev);

    struct FocusPoint: public QPoint {
        int xp, yp;
        Color color;
        bool isActive;
        FocusPoint() { }
        FocusPoint(int x, int y, Color color): QPoint(x, y), color(color), isActive(true) { }
    };

  private:
    quint32 gameSize;
    quint32 colorsSize;
    std::vector<std::vector<GamePoint>> points;
    std::vector<GameRoute> routes;
    FocusPoint* focus;

    quint32 spacing;
    quint32 diameter;

    quint32 currentLevelId;

    Color currentColor;
    GamePoint* currentPoint;

    QSound* connectedSound;
    QSound* breakedSound;

    CompleteDialog* dialog;
    int movesCount;

    void complete(int pointsCount);
    bool dfs(int routeId);

    bool isSoundEnable;
    inline quint32 convertIndexToGridCenterPixel(quint32 index) const;
    inline quint32 convertIndexToPixel(quint32 index) const;
    inline quint32 convertPixelToIndex(quint32 pixel) const;
  signals:
    void nextLevel();
    void completeWith(int movesCount);
  public slots:
    void onLoadLevel(quint32 currentLevelId);
    bool autoSolve();
    void setSound(int setting);
};

#endif // GAMESCENE_H
