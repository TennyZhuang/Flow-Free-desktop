#include "completedialog.h"
#include "ui_completedialog.h"

CompleteDialog::CompleteDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CompleteDialog)
{
    ui->setupUi(this);
}

CompleteDialog::~CompleteDialog()
{
    delete ui;
}

void CompleteDialog::on_cancelButton_clicked()
{
    this->close();
}
