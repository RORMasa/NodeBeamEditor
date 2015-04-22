#include "mainwindow.h"
#include <QApplication>
#include "NodeBeam.h"
#include <QSplashScreen>
#include <QDebug>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug() << *argv;
    QPixmap pixmap(":/images/images/logo.png");
    QSplashScreen splash(pixmap);
    splash.show();

    MainWindow w;
    w.show();

    splash.finish(&w);


    return a.exec();


}
