#include "flowfree.h"
#include "gamescene.h"
#include "ui_flowfree.h"

FlowFree::FlowFree(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FlowFree)
{
    ui->setupUi(this);
    QObject::connect(ui->lastLevel, SIGNAL(clicked(bool)),
                    ui->gameScene, SLOT(loadLastLevel()));

    QObject::connect(ui->nextLevel, SIGNAL(clicked(bool)),
                     ui->gameScene, SLOT(loadNextLevel()));

    QObject::connect(ui->refresh, SIGNAL(clicked(bool)),
                     ui->gameScene, SLOT(clearRoutes()));
}

FlowFree::~FlowFree()
{
    delete ui;
}
