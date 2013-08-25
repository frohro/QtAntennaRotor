#include "mainwindow.h"
#include <QApplication>
#include "rotctlsocket.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
   // RotCtlSocket rotCtlSocket1;

   // rotCtlSocket1.Connect();
    w.show();
    
    return a.exec();
}
