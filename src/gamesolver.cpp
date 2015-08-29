#include "gamesolver.h"
#include <QDebug>
#include <QTime>
#include <QMessageBox>

namespace {

const int directions[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

}

GameSolver::GameSolver(std::vector<std::vector<GamePoint> > points, int colorsSize, QObject *parent):
    points(points),
    gameSize(points.size()),
    colorsSize(colorsSize),
    QThread(parent) {

}

void GameSolver::run() {
    routes.resize(colorsSize + 1, points);

    for (int i = 1; i <= colorsSize; i++) {
        routes[i].setColor((Color)i);
    }

    for (int i = 1; i <= colorsSize; i++) {
        routes[i].clear();
        routes[i].addEndpoint(routes[i].getP1());
    }

    QTime timeCounter;
    timeCounter.start();

    if (dfs(1)) {
        solveTime = timeCounter.elapsed();
    } else {
        timeCounter.elapsed();
        QMessageBox::warning(nullptr, tr("failed"), tr("This level has no solution"), QMessageBox::Cancel);
    }
}
int GameSolver::getSolveTime() const {
    return solveTime;
}


std::vector<GameRoute> GameSolver::getRoutes() const {
    return routes;
}

std::vector<std::vector<GamePoint> > GameSolver::getPoints() const {
    return points;
}


bool GameSolver::dfs(int routeId) {
    if (routeId == colorsSize + 1) {
        int pointsCnt = 0;
        int routeCnt = 0;

        for (int i = 1; i <= colorsSize; i++) {
            if (routes[i].getEndpoints() == 2) {
                ++routeCnt;
            }

            pointsCnt += routes[i].getLength();
        }

        if (routeCnt == colorsSize && pointsCnt == gameSize * gameSize) {
            return true;
        }
    }

    GameRoute* route = &routes[routeId];

    int col1 = route->getPoints().back()->col;
    int row1 = route->getPoints().back()->row;

    for (int i = 0; i < 4; i++) {
        int row2 = row1 + directions[i][0];
        int col2 = col1 + directions[i][1];

        GamePoint* point = &points[row2][col2];

        if (row2 < gameSize && row2 >= 0 && col2 < gameSize && col2 >= 0) {
            if (*point == *(route->getP2())) {
                route->addEndpoint(point);

                if (dfs(routeId + 1)) {
                    return true;
                }

                route->popPoint();
            } else if (!(bool)point->color) {
                point->color = route->getColor();
                route->addPoint(point);

                if (dfs(routeId)) {
                    return true;
                }

                route->popPoint();
                point->color = Color::NONE;
            }
        }
    }

    return false;
}


