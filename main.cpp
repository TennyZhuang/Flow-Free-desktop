#include "flowfree.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FlowFree w;
    w.show();

    return a.exec();
}
