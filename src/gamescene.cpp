#include "common.h"
#include "gamescene.h"
#include "gamesolver.h"
#include <QPainter>
#include <QPen>
#include <QLabel>
#include <QDebug>
#include <QColor>
#include <QMouseEvent>
#include <QDialog>
#include <QMessageBox>
#include <assert.h>

// some helper function, only used in this file
namespace {

// convert Color defined by coder to QColor
QColor convertToQColor(Color color) {
    switch(color) {
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

// check if point is overbound
inline bool overBound(int x, int y) {
    return (x >= SCEAN_SIZE || x < 0 ||
            y >= SCEAN_SIZE || y < 0);
}

// four directions used in auto solve
const int directions[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

} // anonymous namespace

GameScene::GameScene(QWidget *parent) : QWidget(parent) {
    connectedSound = new QSound(":/sounds/connect.wav");
    breakedSound = new QSound(":/sounds/break.wav");
    isSoundEnable = true;
    dialog = new CompleteDialog;
    timer = new QTimer;
    timer->setInterval(1000);

    connect(timer, &QTimer::timeout,
    [=]() {
        emit timeChanged(++currentTime);
    });

    connect(this, SIGNAL(completeWith(int,int)),
            dialog, SLOT(changeText(int,int)));
}

GameScene::~GameScene() {
    delete connectedSound;
    delete breakedSound;
    delete dialog;

    if (currentPoint) {
        delete currentPoint;
    }
}

void GameScene::paintEvent(QPaintEvent *ev) {
    // draw background
    QPainter p(this);
    p.fillRect(0, 0, SCEAN_SIZE, SCEAN_SIZE, Qt::black);

    // draw grids
    p.setPen(QPen(QColor(100, 255, 218), 2));

    for (quint32 i = 0; i <= gameSize; i++) {
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
    for (int i = 1; i <= colorsSize; i++) {
        const auto& points = routes[i].getPoints();

        if (points.size() < 1) {
            continue;
        }


        QPen pen(convertToQColor(routes[i].getColor()), 20);
        pen.setJoinStyle(Qt::RoundJoin);
        p.setPen(pen);

        for (int j = 0; j < points.size() - 1; j++) {
            p.drawLine(convertIndexToGridCenterPixel(points[j]->col),
                       convertIndexToGridCenterPixel(points[j]->row),
                       convertIndexToGridCenterPixel(points[j + 1]->col),
                       convertIndexToGridCenterPixel(points[j + 1]->row));
        }

        p.setPen(QPen(Qt::transparent, 0));
        auto qColor = convertToQColor(routes[i].getColor());
        qColor.setAlphaF(0.4);

        if (i == (int)currentColor) {
            continue;
        }

        for (auto& point: points) {
            p.fillRect(convertIndexToPixel(point->col),
                       convertIndexToPixel(point->row),
                       spacing,
                       spacing,
                       qColor);
        }
    }

    // draw point
    p.setPen(QPen(Qt::transparent, 0));

    for (const auto& pointRow: points) {
        for (const auto& point: pointRow) {
            if ((bool)point.color && point.isEndpoint) {
                p.setBrush(QBrush(convertToQColor(point.color)));
                p.drawEllipse(QPoint(convertIndexToGridCenterPixel(point.col),
                                     convertIndexToGridCenterPixel(point.row)),
                              diameter >> 1, diameter >> 1);
            }
        }
    }

    // draw foucus
    if (focus) {
        QColor qcolor = convertToQColor(focus->color);
        qcolor.setAlphaF((focus->isActive)? 0.6: 0.3);
        p.setBrush(qcolor);
        p.drawEllipse(*focus, diameter, diameter);
    }
}

void GameScene::mousePressEvent(QMouseEvent* ev) {
    if (overBound(ev->x(), ev->y())) {
        return;
    }

    setCursor(Qt::OpenHandCursor);
    currentPoint = &points[convertPixelToIndex(ev->y())][convertPixelToIndex(ev->x())];

    if ((bool)currentPoint->color) {
        if (currentColor != currentPoint->color) {
            ++movesCount;
        }

        currentColor = currentPoint->color;
        focus = new FocusPoint(ev->x(), ev->y(),
                               currentPoint->color);

        if (currentPoint->isEndpoint) {
            routes[(int)currentColor].clear();
            routes[(int)currentColor].addEndpoint(currentPoint);
        } else {
            routes[(int)currentColor].eraseAfter(currentPoint, true);
        }
    }

    update();
}

void GameScene::mouseMoveEvent(QMouseEvent *ev) {
    // over screen
    if (overBound(ev->x(), ev->y())) {
        return;
    }

    // update focus
    if (focus) {
        focus->setX(ev->x());
        focus->setY(ev->y());
    } else {
        return;
    }

    // check point update
    int tempRow = convertPixelToIndex(ev->y());
    int tempCol = convertPixelToIndex(ev->x());

    if (tempRow == currentPoint->row && tempCol == currentPoint->col) {
        // point not changed
        update();
        return;
    }

    // overstep
    if (abs(currentPoint->col - tempCol) + abs(currentPoint->row - tempRow) > 1) {
        focus->isActive = false;
        update();
        return;
    }

    if (tempCol >= gameSize || tempCol < 0 || tempRow >= gameSize || tempRow < 0) {
        update();
        return;
    }

    GamePoint* tempPoint = &points[tempRow][tempCol];

    if ((bool)tempPoint->color) {
        // have color
        if (currentPoint->isEndpoint && routes[(int)currentColor].getLength() > 1 && tempPoint->color != currentColor) {
            update();
            return;
        }

        if (tempPoint->isEndpoint) {
            // is end point
            if (tempPoint->color == currentColor) {
                // same color
                if (tempPoint == routes[(int)currentColor].getPoints()[0]) {
                    // start point
                    // restart
                    routes[(int)currentColor].clear();
                } else {
                    // end point
                }

                routes[(int)currentColor].addEndpoint(tempPoint);
                currentPoint = tempPoint;
            } else {
                // differenet color
                // prevent default
                focus->isActive = false;
            }

            update();
            return;
        } else {
            // not end point
            // break current route
            if (tempPoint->color != currentColor)
                if (isSoundEnable) {
                    breakedSound->play();
                }

            routes[(int)tempPoint->color].eraseAfter(tempPoint);
        }
    }

    focus->isActive = true;

    if (routes[(int)currentColor].getEndpoints() == 2) {
        update();
        return;
    }

    if (!(*((routes[(int)currentColor]).getPoints().end() - 1) == tempPoint)) {
        routes[(int)currentColor].addPoint(&points[tempRow][tempCol]);
    }

    tempPoint->color = currentColor;
    currentPoint = &points[tempRow][tempCol];

    update();
}

void GameScene::mouseReleaseEvent(QMouseEvent *ev) {
    if (focus) {
        delete focus;
    }

    if (routes[(int)currentColor].getEndpoints() == 2)
        if (isSoundEnable) {
            connectedSound->play();
        }

    focus = nullptr;
    setCursor(Qt::ArrowCursor);
    currentColor = Color::NONE;
    currentPoint = nullptr;

    // check win or not win
    {
        int pointsCnt = 0;
        int routesCnt = 0;

        for (const auto& route: routes) {
            if (route.getEndpoints() == 2) {
                ++routesCnt;
            }

            pointsCnt += route.getLength();
            qDebug() << routesCnt << pointsCnt;
        }

        if (routesCnt == colorsSize) {
            complete(pointsCnt);
        }
    }

    update();
}

void GameScene::complete(int pointsCount) {
    if (pointsCount == gameSize * gameSize) {
        emit completeWith(movesCount, currentTime);

        if (dialog->exec()) {
            emit nextLevel();
        }
    } else {
        QMessageBox::about(nullptr, tr("alert"), tr("You should <b>fill</b> the puzzle with pipes."));
    }
}

bool GameScene::autoSolve() {
    while (solver);

    points = answerPoints;
    routes = answerRoutes;
    movesCount = colorsSize;
    update();

    return true;
}

void GameScene::setSound(int setting) {
    isSoundEnable = (bool)setting;
}

void GameScene::onSolved() {
    qDebug() << "1234";
    answerPoints = solver->getPoints();
    answerRoutes = solver->getRoutes();
    qDebug() << answerPoints.size();
    qDebug() << answerRoutes.size();
//    delete solver;
    solver = nullptr;
    emit enableResultButton();
}

inline quint32 GameScene::convertIndexToGridCenterPixel(quint32 index) const {
    return SCEAN_PADDING + 2 + (index + 0.5) * spacing;
}

inline quint32 GameScene::convertIndexToPixel(quint32 index) const {
    return SCEAN_PADDING + 2 + index * spacing;
}

inline quint32 GameScene::convertPixelToIndex(quint32 pixel) const {
    if (pixel < SCEAN_PADDING) {
        return 0;
    }

    if (pixel >= SCEAN_SIZE - SCEAN_PADDING) {
        return gameSize - 1;
    }

    return (pixel - SCEAN_PADDING) / spacing;
}

void GameScene::onLoadLevel(quint32 currentLevelId) {
    focus = nullptr;
    auto gameModal = GameModel::instance();
    assert(currentLevelId > 0 || currentLevelId <= gameModal->size());
    movesCount = 0;
    currentTime = 0;
    emit timeChanged(currentTime);
    timer->start();
    const Level level = gameModal->getLevel(currentLevelId);
    points = level.getPoints();
    gameSize = level.getSize();
    colorsSize = level.getColorsSize();

    solver = new GameSolver(points, colorsSize);
    connect(solver, &GameSolver::finished,
            this, &GameScene::onSolved);

    solver->start();
//    solver->wait();

    spacing = (SCEAN_SIZE - SCEAN_PADDING * 2) / gameSize;
    diameter = spacing * 0.8;

    if (this->currentLevelId == currentLevelId) {
        if (isSoundEnable) {
            breakedSound->play();
        }
    } else {
        this->currentLevelId = currentLevelId;
    }

    routes.clear();
    routes.resize(colorsSize + 1, points);

    for (int i = 1; i <= colorsSize; i++) {
        routes[i].setColor((Color)i);
    }

    currentPoint = nullptr;
    update();
}
