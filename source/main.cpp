#include "mainwindow.h"
#include <QApplication>
#include "NodeBeam.h"
#include <QSplashScreen>
#include <QDebug>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap pixmap(":/images/images/logo.png");
    QSplashScreen splash(pixmap);
    splash.show();

    MainWindow w;
    w.show();
    splash.finish(&w);

    if(argc>1)
    {
        w.OpenJBEAM_cmdargument(QString::fromLatin1(argv[1]));
    }

    return a.exec();


}
