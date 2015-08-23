#ifndef FLOWFREE_H
#define FLOWFREE_H

#include <QMainWindow>

namespace Ui {
class FlowFree;
}

class FlowFree : public QMainWindow
{
    Q_OBJECT

public:
    explicit FlowFree(QWidget *parent = 0);
    ~FlowFree();    

signals:
    void lastLevel();

    void nextLevel();

private slots:
    void on_lastLevel_clicked();

    void on_nextLevel_clicked();

private:
    Ui::FlowFree *ui;
};

#endif // FLOWFREE_H
