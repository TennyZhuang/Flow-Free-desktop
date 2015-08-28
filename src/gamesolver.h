#ifndef GAMESOLVER_H
#define GAMESOLVER_H

#include "gamepoint.h"
#include "gameroute.h"
#include <QThread>

class GameSolver : public QThread {
    Q_OBJECT
  public:
    explicit GameSolver(std::vector<std::vector<GamePoint>> points, int colorsSize, QObject *parent = 0);

    std::vector<std::vector<GamePoint> > getPoints() const;

    std::vector<GameRoute> getRoutes() const;

  protected:
    void run();

  private:
    std::vector<std::vector<GamePoint>> points;
    std::vector<GameRoute> routes;
    int gameSize;
    int colorsSize;

    bool dfs(int routeId);
  signals:

  public slots:
};

#endif // GAMESOLVER_H
