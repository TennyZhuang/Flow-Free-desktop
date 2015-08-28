#include "completedialog.h"
#include "ui_completedialog.h"
#include "ui_flowfree.h"

CompleteDialog::CompleteDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CompleteDialog) {
    ui->setupUi(this);
}

CompleteDialog::~CompleteDialog() {
    delete ui;
}

void CompleteDialog::changeText(int movesCount) {
    QString prefix = "You solved this level in ";
    QString suffix = " moves";
    ui->label->setText(prefix + QString::number(movesCount) + suffix);
}

void CompleteDialog::on_cancelButton_clicked() {
    emit reject();
}

void CompleteDialog::on_nextLevelButton_clicked() {
    emit accept();
}
