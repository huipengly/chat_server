#ifndef MY_TCP_SERVER_H
#define MY_TCP_SERVER_H

#include <QtNetwork>
#include <vector>

class my_tcp_server : public QObject
{
    Q_OBJECT
    QTcpServer *serverSock;
    QTcpSocket *clientSock;

public:
    my_tcp_server	( QObject *parent = NULL );
    ~my_tcp_server ();

//    void run(quint16 port);

private slots:
    void acceptConnection();    //接收连接函数
    void receiveData();         //接收数据函数
    void disConnection();       //断开连接函数

};

#endif // MY_TCP_SERVER_H
