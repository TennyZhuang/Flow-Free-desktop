#include "flowfree.h"
#include "gamescene.h"
#include "ui_flowfree.h"

FlowFree::FlowFree(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FlowFree)
{
    ui->setupUi(this);
    QObject::connect(this, SIGNAL(lastLevel()),
                    ui->gameScene, SLOT(loadLastLevel()));

    QObject::connect(this, SIGNAL(nextLevel()),
                     ui->gameScene, SLOT(loadNextLevel()));
}

FlowFree::~FlowFree()
{
    delete ui;
}

void FlowFree::on_lastLevel_clicked()
{
    emit lastLevel();
}

void FlowFree::on_nextLevel_clicked()
{
    emit nextLevel();
}
