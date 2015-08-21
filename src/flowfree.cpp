#include "flowfree.h"
#include "ui_flowfree.h"

FlowFree::FlowFree(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FlowFree)
{
    ui->setupUi(this);
}

FlowFree::~FlowFree()
{
    delete ui;
}
