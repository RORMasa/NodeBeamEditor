#include "mainwindow.h"
#include <QApplication>
#include "NodeBeam.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
