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
    void loadLevel(quint32 levelId);
private slots:

private:
    Ui::FlowFree *ui;
    quint32 currentLevelId;
};

#endif // FLOWFREE_H
