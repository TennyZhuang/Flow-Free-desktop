#include "flowfree.h"
#include "gamescene.h"
#include "gamemodel.h"
#include "ui_flowfree.h"
#include <QDebug>
#include <QFile>

FlowFree::FlowFree(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FlowFree) {
    ui->setupUi(this);

    QFile *file = new QFile(":/stylesheets/button.qss");
        file->open(QFile::ReadOnly);
        setStyleSheet(file->readAll());
        file->deleteLater();

    quint32 gameSize = GameModel::instance()->size();
    connect(this, SIGNAL(loadLevel(quint32)),
            ui->gameScene, SLOT(onLoadLevel(quint32)));

    connect(ui->lastLevelButton, &QPushButton::clicked,
    [=]() {
        emit loadLevel(--currentLevelId);
        ui->resultButton->setEnabled(false);
        ui->lastLevelButton->setEnabled(currentLevelId != 1);
        ui->nextLevelButton->setEnabled(currentLevelId != gameSize);
    });

    connect(ui->reloadButton, &QPushButton::clicked,
    [=]() {
        emit loadLevel(currentLevelId);
        ui->resultButton->setEnabled(false);
    });

    connect(ui->nextLevelButton, &QPushButton::clicked,
    [=]() {
        emit loadLevel(++currentLevelId);
        ui->resultButton->setEnabled(false);
        ui->lastLevelButton->setEnabled(currentLevelId != 1);
        ui->nextLevelButton->setEnabled(currentLevelId != gameSize);
    });

    connect(ui->resultButton, &QPushButton::clicked,
            ui->gameScene, &GameScene::autoSolve);

    connect(ui->resultButton, &QPushButton::clicked,
    [=]() {
        ui->resultButton->setEnabled(false);
    });

    connect(ui->gameScene, &GameScene::nextLevel,
    [=]() {
        ui->nextLevelButton->click();
    });

    connect(ui->soundCheckBox, &QCheckBox::stateChanged,
            ui->gameScene, &GameScene::setSound);

    connect(ui->gameScene, SIGNAL(timeChanged(int)),
            ui->lcdNumber, SLOT(display(int)));

    connect(ui->gameScene, &GameScene::enableResultButton,
    [=]() {
        ui->resultButton->setEnabled(true);
    });

    currentLevelId = 0;
    ui->nextLevelButton->click();
}

FlowFree::~FlowFree() {
    delete ui;
}
