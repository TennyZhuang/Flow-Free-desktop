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

void CompleteDialog::changeText(int movesCount, int completeTime) {
    ui->label->setText(QString("You finished this level in ") +
                       QString::number(movesCount) +
                       QString(" moves and ") +
                       QString::number(completeTime) +
                       QString(" seconds"));
}

void CompleteDialog::on_cancelButton_clicked() {
    emit reject();
}

void CompleteDialog::on_nextLevelButton_clicked() {
    emit accept();
}
