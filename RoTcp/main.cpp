#include "RoTcp.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RoTcp w;
    w.show();

    return a.exec();
}
