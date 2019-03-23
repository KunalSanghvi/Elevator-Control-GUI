#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QList>


class TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0, uint16_t portNum = 0);

    
    QString inBuffer;
    QString finalData;
    int printEnable;
    int stringLength ;
    int startPointer ;
    int dataSize ;
    int writePointer ;
    int readPointer ;
    int bufferSize ;
    int bufferMaxSize;
    QString buffer[255];
    QByteArray byteArrayBuffer;
    int clientConnect;

    int ip_data_available();
    int get_bufferSize();
    void clear_buffer();
    QString read_buffer();
    int write_buffer(QString tempData);
    int server_write(QString tempData);
    
signals:
    void messageRecv(QString message);

public slots:
    void new_connection();
    void read_connection();
    void my_ready_read();

private:
    QTcpServer *server;
    QTcpSocket *connectedClient;
    QTimer *bufferReadTime;
    QList<QTcpSocket*> *list;
};


#endif // TCPSERVER_H
