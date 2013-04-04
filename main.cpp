#include <QtGui/QApplication>
#include <QtGui/QSplashScreen>
#include "classes/mainwindow/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSplashScreen *splash=new QSplashScreen;
    splash->setPixmap(QPixmap(":/images/splash.png"));
    splash->show();
    Qt::Alignment topLeft= Qt::AlignTop | Qt::AlignLeft;
    splash->showMessage(QObject::tr("Setting up Main Window..."),
                        topLeft);
    MainWindow w;
    splash->showMessage(QObject::tr("Main Window is ready to go..."),
                        topLeft);
    w.show();
    splash->finish(&w);
    delete splash;
    
    return a.exec();
}
