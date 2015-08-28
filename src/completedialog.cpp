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

void CompleteDialog::on_cancelButton_clicked() {
    emit reject();
}

void CompleteDialog::on_nextLevelButton_clicked() {
    emit accept();
}
