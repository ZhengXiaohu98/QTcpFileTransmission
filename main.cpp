#include "serverwidget.h"
#include "clientwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ServerWidget w;
    ClientWidget w1;
    w.show();
    w1.show();
    return a.exec();
}
