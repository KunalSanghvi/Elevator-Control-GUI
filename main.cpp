#include "mainwindow.h"
#include <QApplication>

int myTime; //change

int main(int argc, char *argv[])
{
	myTime = 0; //change
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    w.show();

    return a.exec();
}
