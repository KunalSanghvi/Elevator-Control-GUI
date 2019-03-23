// TcpServer.cpp

#include "TcpServer.h"

TcpServer::TcpServer(QObject *parent, uint16_t portNum) :
    QObject(parent)
{
    //variable init
    stringLength = 0;
    startPointer = 0;
    dataSize = 0;
    writePointer = 0;
    readPointer = 0;
    bufferSize = 0;
    bufferMaxSize = 250;
    printEnable = 0;
    clientConnect = 0;

    //sever init
    server = new QTcpServer(this);
    connectedClient = new QTcpSocket(this);
    list = new QList<QTcpSocket*>;
    connect(server, SIGNAL(newConnection()), this, SLOT(new_connection()));

    if(!server->listen(QHostAddress::Any, portNum))       //pass host address in constructer
    {
        qDebug() << "Server could not start!";
    }
    else
    {
        qDebug() << "Server started!";
    }

    // //init of timer to read data from tcp buffer
    bufferReadTime = new QTimer(this);
    connect(bufferReadTime , SIGNAL(timeout()), this, SLOT(read_connection()));
    bufferReadTime->start(10);                       //reading data after every 10ms
}

void TcpServer::new_connection()
{
    connectedClient = server->nextPendingConnection();
    // clientConnect = 1;                                   //remove list if not used
    // list->append(server->nextPendingConnection());
    // qDebug() << "server->nextPendingConnection(): ";        
    // qDebug() << list;        
    // qDebug() << "above is the value ";        

    this->server_write("Hello Client");
    this->server_write("1233~:39940[]}/.,+=*&^%$");


    // list->last()->flush();
    
}

void TcpServer::my_ready_read()
{
    if(clientConnect == 1)
    {
        connect(list->last(),SIGNAL(readyRead()),this,SLOT(read_connection()));
        clientConnect = 0;        
    }
    else
    {

    }
   /* while(list->last()->bytesAvailable() > 0)
    {
        // qDebug() << "list->last()->waitForReadyRead: ";
        // qDebug() << list->last()->waitForReadyRead();
        // if (!list->last()->waitForReadyRead(60)) 
        // {
        //  // timeout after 30 second, by default
        //     qDebug() << "waitForReadyRead() timed out";
        //     break;
        // }

    }*/
    /*while(list->last()->bytesAvailable() > 0)
    {
        QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
        byteArrayBuffer = socket->readAll();

        inBuffer += QString::fromAscii(byteArrayBuffer.data());        
    }*/
    // //init of timer to read data from tcp buffer
    // bufferReadTime = new QTimer(this);
    // connect(bufferReadTime , SIGNAL(timeout()), this, SLOT(read_connection()));
    // bufferReadTime->start(10);                       //reading data after every 10ms
    
    // qDebug() << "read connection Raw Data: "+inBuffer;         
}

void TcpServer::read_connection()
{    
    int retVal = this->ip_data_available();
    //qDebug() << retVal;
    if(retVal > 0)
    {
        QString recvData = this->read_buffer();
        emit messageRecv(recvData);
        // qDebug() << "YAyyy Got data: " + recvData;
    }
    else
    {
        // qDebug() << "No data in buffer";        
    }
}

int TcpServer::ip_data_available()
{
    int searchLocation = 0;    


    if(connectedClient != NULL)
    {
        inBuffer += connectedClient->readAll();
        // QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());

        // byteArrayBuffer = socket->readAll();
        //QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
        // qDebug() << "socket: ";            
        // qDebug() << socket;            

        // inBuffer += QString::fromAscii(byteArrayBuffer.data());

        stringLength = inBuffer.length();



        if(printEnable == 1 && inBuffer.isEmpty() == false)
        {
            qDebug() << "Raw Data: "+inBuffer;            
        }


        for(int i = 1; i < (stringLength+1); i++)                   //search for start bit until string is finished
        {
            startPointer = inBuffer.indexOf(":",searchLocation);
            if(startPointer > -1)                                 //if start bit found
            {                
                startPointer++;
                if((startPointer) == stringLength)            //if data finished before getting 2nd start bit
                {
                   
                    inBuffer = ":";
                    return get_bufferSize();
                }
                else                                        //if data not finished
                {
                    QString startBit2 = inBuffer.at(startPointer);
                    
                    if(startBit2 == "~")                    //if 2nd start bit found
                    {
                        startPointer++;
                        if((startPointer) == stringLength) //if data finished before getting 2nd start bit
                        {
                            if(printEnable == 1)
                            {
                                qDebug() << "TCPServerProtocol : startBit2 found but packet finished";
                                qDebug() << "Data : " + inBuffer;
                            }
                            inBuffer = ":~";
                            return get_bufferSize();
                        }
                        else                                //if data not finished
                        {
                            QString dataSizeTemp = inBuffer.at(startPointer);
                            startPointer++;
                            if((startPointer) == stringLength) //if data finished before getting 2nd datasize
                            {
                                if(printEnable == 1)
                                {
                                    qDebug() << "TCPServerProtocol :first data size char found but packet finished";
                                    qDebug() << "Data : " + inBuffer;
                                }
                                inBuffer = ":~";
                                inBuffer += dataSizeTemp;
                                return get_bufferSize();
                            }
                            else
                            {
                                dataSizeTemp += (inBuffer.at(startPointer));
                                startPointer++;
                                if((startPointer) == stringLength) //if data finished before getting 2nd datasize
                                {
                                    if(printEnable == 1)
                                    {
                                        qDebug() << "TCPServerProtocol : second data size char found but packet finished";
                                        qDebug() << "Data : " + inBuffer;
                                    }
                                    inBuffer = ":~";
                                    inBuffer += dataSizeTemp;
                                    return get_bufferSize();
                                }
                                else
                                {
                                    dataSizeTemp += (inBuffer.at(startPointer));                                    
                                    dataSize = dataSizeTemp.toInt();
                                    startPointer++;
                                    if(dataSize == 0)
                                    {
                                        if((startPointer) == stringLength) //if data finished before getting 2nd start bit
                                        {
                                            if(printEnable == 1)
                                            {
                                                qDebug() << "TCPServerProtocol error: datasize is wrong and data got empty";
                                                qDebug() << "Data : " + inBuffer;
                                            }
                                            inBuffer = "";
                                            return get_bufferSize();
                                        }
                                        else
                                        {
                                            if(printEnable == 1)
                                            {
                                                qDebug() << "TCPServerProtocol error: datasize is wrong ";
                                                qDebug() << "Data : " + inBuffer;
                                            }
                                            searchLocation = startPointer;
                                            i = startPointer - 1;
                                        }
                                    }

                                    else if(dataSize >=  (stringLength - startPointer)) //if left data is small than data size
                                    {
                                        QString tempBuffer = ":~";
                                        tempBuffer += dataSizeTemp;
                                        for(int z = startPointer; z < stringLength; z++)
                                        {
                                            tempBuffer = tempBuffer + inBuffer.at(z);
                                        }
                                        inBuffer = tempBuffer;
                                        if(printEnable == 1)
                                        {
                                            qDebug() << "TCPServerProtocol : data found but packet finished before STOP_BIT";
                                            qDebug() << "Data : " + inBuffer;
                                        }
                                        return get_bufferSize();
                                    }

                                    else                                //if data not finished
                                    {
                                        QString stopBit = inBuffer.at(startPointer + dataSize);
                                        if(stopBit == "~")    
                                        {
                                            finalData = "";
                                            for(int z = startPointer; z < (startPointer + dataSize) ; z++)
                                            {
                                                finalData = finalData + inBuffer.at(z);
                                            }
                                            //qDebug() << finalData;
                                            if(write_buffer(finalData) == 1)
                                            {
                                                if(printEnable == 1)
                                                {
                                                    qDebug() << "TCPServerProtocol : Data stored";
                                                    qDebug() << "Data : " + finalData;
                                                }
                                            }
                                            else
                                            {
                                                if(printEnable == 1)
                                                {
                                                    qDebug() << "TCPServerProtocol Error: buffer full";
                                                }
                                                return get_bufferSize();
                                            }                                            
                                            searchLocation = startPointer;
                                            i = startPointer - 1;
                                        }
                                        else
                                        {
                                            if(printEnable == 1)
                                            {
                                                qDebug() << "TCPServerProtocol : Didnt found stop bit";
                                                qDebug() << "Data : " + finalData;
                                            }
                                            searchLocation = startPointer;
                                            i = startPointer - 1;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else                                    //if no 2ndstart bit found
                    {
                        searchLocation = startPointer;
                        i = startPointer - 1;
                    }
                }
            }
            else    //if no start bit found
            {
                if(printEnable == 1)
                {
                    qDebug() << "TCPServerProtocol Error : Garbage packet";
                    qDebug() << "Data : " + inBuffer;
                }
                inBuffer = "";
                return get_bufferSize();
            }
        }
    }
    else
    {
        return get_bufferSize();
    }
    return get_bufferSize();
}

int TcpServer::get_bufferSize()
{
    int difference;
    if( writePointer >= readPointer)
    {
        difference = writePointer - readPointer; 
    }
    else
    {
        difference = bufferMaxSize - readPointer; 
        difference = difference + writePointer; 
    }

    return difference;
}

QString TcpServer::read_buffer()
{
    if(get_bufferSize() > 0)
    {
        QString readData = buffer[readPointer];
        readPointer++;
        if(readPointer > bufferMaxSize)
        {
            readPointer = 0;
        }
        // qDebug() << "Data in read buffer: "+readData;
        return readData;
    }
    else
    {       
        return "";
    }
}

int TcpServer::write_buffer(QString tempData)
{
    if(get_bufferSize() < (bufferMaxSize + 1))
    {
        buffer[writePointer] = tempData;
        writePointer++;
        if(writePointer > bufferMaxSize)
        {
            writePointer = 0;
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

int TcpServer::server_write(QString tempData)
{
    try 
    {
        QString tempData1 = ":~";
        if(tempData.length() < 10 )
        {
            tempData1 += "00";
            tempData1 += QString::number(tempData.length());
            tempData1 += tempData;
            tempData1 += "~"; 
        }
        else if(tempData.length() < 100)
        {
            tempData1 += "0";
            tempData1 += QString::number(tempData.length());
            tempData1 += tempData;
            tempData1 += "~"; 
        }
        else if(tempData.length() < 1000)
        {
            tempData1 += QString::number(tempData.length());
            tempData1 += tempData;
            tempData1 += "~"; 
        }
        // QByteArray tempByteArray = tempData1.toLatin1();
        // const char *c_str2 = tempByteArray.data(); 
        // qDebug() << c_str2;
        connectedClient->write(tempData1.toLocal8Bit());
        return 1;
    }
    catch (...) 
    {
        qDebug() << "TCPServerProtocol Error : Client not present ";
        return 0;

    }
}

void TcpServer::clear_buffer()
{
    writePointer = 0;
    readPointer = 0;
}


