#include "flowfree.h"
#include "gamescene.h"
#include "gamemodel.h"
#include "ui_flowfree.h"
#include <QDebug>

FlowFree::FlowFree(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FlowFree)
{
    ui->setupUi(this);
    quint32 gameSize = GameModel::instance()->size();
    QObject::connect(this, SIGNAL(loadLevel(quint32)),
                     ui->gameScene, SLOT(onLoadLevel(quint32)));

    QObject::connect(ui->lastLevelButton, &QPushButton::clicked,
                     [=]()
    {
        emit loadLevel(--currentLevelId);
        ui->lastLevelButton->setEnabled(currentLevelId != 1);
        ui->nextLevelButton->setEnabled(currentLevelId != gameSize);
    });

    QObject::connect(ui->reloadButton, &QPushButton::clicked,
                     [=]() { emit loadLevel(currentLevelId); });

    QObject::connect(ui->nextLevelButton, &QPushButton::clicked,
                     [=]()
    {
        emit loadLevel(++currentLevelId);
        ui->lastLevelButton->setEnabled(currentLevelId != 1);
        ui->nextLevelButton->setEnabled(currentLevelId != gameSize);
    });

    currentLevelId = 0;
    ui->nextLevelButton->click();
//    emit loadLevel(++currentLevelId);
//    ui->lastLevelButton->setEnabled(currentLevelId != 1);
}

FlowFree::~FlowFree()
{
    delete ui;
}
