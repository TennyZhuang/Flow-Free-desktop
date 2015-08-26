#ifndef COMPLETEDIALOG_H
#define COMPLETEDIALOG_H

#include <QDialog>

namespace Ui {
class CompleteDialog;
}

class CompleteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CompleteDialog(QWidget *parent = 0);
    ~CompleteDialog();

private slots:
    void on_cancelButton_clicked();

private:
    Ui::CompleteDialog *ui;
};

#endif // COMPLETEDIALOG_H