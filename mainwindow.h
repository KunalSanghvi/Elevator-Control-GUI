#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <stdint.h>

#include "mywidget.h"
#include "TcpServer.h"
#include "global.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
public slots:
    void get_recv_data(QString dataTcp);

private slots:
    void update_paint_event();
    void update_data(); //kunal change
    void on_OK_clicked();
    void wait_time(void);

private: 
    Ui::MainWindow *ui;
    QTimer *drawTimer;
    QTimer *cpTimer;
    QTimer *dataTimer; //kunal change
    QTimer *delayTimer; 
    mywidget *myWidgetLine;
    TcpServer *myServer;
};

#endif // MAINWINDOW_H
