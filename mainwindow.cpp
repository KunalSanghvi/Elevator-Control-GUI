#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->myServer = new TcpServer(this, 1234);
    
    myWidgetLine = new mywidget(this);
    ui->verticalLayout_2->addWidget(myWidgetLine);

    drawTimer = new QTimer(this); //refreshing Frame speed
    connect(drawTimer, SIGNAL(timeout()), this, SLOT(update_paint_event()));
    drawTimer->start(10);

    delayTimer = new QTimer(this); //For millis
    connect(delayTimer, SIGNAL(timeout()), this, SLOT(wait_time()));
    delayTimer->start(1);

    dataTimer = new QTimer(this);
    connect(dataTimer, SIGNAL(timeout()), this, SLOT(update_data()));
    dataTimer->start(10);

    connect(this->myServer, SIGNAL(messageRecv(QString)), this, SLOT(get_recv_data(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update_paint_event()
{
	this->myWidgetLine->update();
}

void MainWindow::on_OK_clicked() //kunal change
{
    QString arrStr = ui->ArrivalFloor->text();

    QString destStr = ui->DestinationFloor->text();

    this->myServer->server_write(arrStr);
    this->myServer->server_write(destStr);
} 

void MainWindow::get_recv_data(QString dataTcp)
{
    int tcpData = dataTcp.toInt();
    qDebug() << tcpData;
    int quotient = tcpData/10;
    int remainder = tcpData%10;
    if(quotient == 1)
    {
        myWidgetLine->liftMove1->targetPosition = remainder;
    }
    else if(quotient == 2)
    {
        myWidgetLine->liftMove2->targetPosition = remainder;
    }
    else if(quotient == 3)
    {
        myWidgetLine->liftMove3->targetPosition = remainder;
    }
    else if(quotient == 4)
    {
        myWidgetLine->liftMove4->targetPosition = remainder;
    }
}

void MainWindow::update_data()
{
    this->myServer->server_write(QString::number(myWidgetLine->liftMove1->currentPosition));
    this->myServer->server_write(QString::number(myWidgetLine->liftMove2->currentPosition));
    this->myServer->server_write(QString::number(myWidgetLine->liftMove3->currentPosition));
    this->myServer->server_write(QString::number(myWidgetLine->liftMove4->currentPosition));
    
    this->myServer->server_write(QString::number(myWidgetLine->status1));
    this->myServer->server_write(QString::number(myWidgetLine->status2));
    this->myServer->server_write(QString::number(myWidgetLine->status3));
    this->myServer->server_write(QString::number(myWidgetLine->status4));
}

void MainWindow::wait_time(void)
{
    myTime++;
}