#include "my_tcp_server.h"
#include <iostream>
#include <string>
#include <QString>

using namespace std;

bool is_admin_online;
bool is_test_online;
QString admin_ip;
QString test_ip;

my_tcp_server::my_tcp_server(QObject *parent)
    : QObject( parent )
{
    serverSock = new QTcpServer(this);
    clientSock = NULL;

    is_admin_online = false;
    is_test_online = false;
//    is_admin_online = 0;
//    is_test_online = 0;

    if(!serverSock->listen(QHostAddress::Any,32146))
    {
        qDebug()<<serverSock->errorString();
    }

    qDebug() << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss") << "  "
             << "服务器启动，监听中....";

    connect(serverSock,SIGNAL(newConnection()),this,SLOT(acceptConnection()));//当有新连接的时候接受连接
}

my_tcp_server::~my_tcp_server()
{
    delete serverSock;
    delete clientSock;
}

void my_tcp_server::acceptConnection()
{
    //得到客户端socket号
    clientSock = serverSock->nextPendingConnection();

    qDebug() << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss") << "  "
             << "客户端ip:"  << clientSock->peerAddress().toString()
             << ",port:"  << QString::number(clientSock->peerPort(), 10)
             << "连接到服务器";

    //客户端触发
    connect(clientSock,SIGNAL(disconnected()),this,SLOT(disConnection()));//断开连接触发断开函数
    connect(clientSock,SIGNAL(readyRead()),this,SLOT(receiveData()));//收到数据触发接收函数
}

void my_tcp_server::receiveData()
{
    qint64 len = 0;
    while((len = clientSock->bytesAvailable()) != 0)
    {
        QByteArray recData;
        recData = clientSock->readLine();
        QString msg = QString::fromLocal8Bit(recData);

        //接收的信息输出在屏幕上
        qDebug() << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss") << "  "
                 << "客户端ip:" << clientSock->peerAddress().toString()
                 << ",port:"   << QString::number(clientSock->peerPort(), 10)
                 << "收到:" << msg;

        if(clientSock != NULL)
        {
            QString state_return = "false";
            if(!state_return.isEmpty())
            {
                if(msg == "admin#admin" )//判定用户名密码是否正确
                {
                    if(is_admin_online == false)
                    {
//                        if(is_test_online == false)
//                            state_return = "true";
//                        else
//                            state_return = "true&";
                        state_return = "true";
                        is_admin_online = true;
                        admin_ip = clientSock->peerAddress().toString();
                    }
                }
                if(msg == "test#test")
                {
                    if(is_test_online == false)
                    {
//                        if(is_admin_online == false)
//                            state_return = "true";
//                        else
//                            state_return = "true&";
                        state_return = "true";
                        is_test_online = true;
                        test_ip = clientSock->peerAddress().toString();
                    }
                }
                if(msg == "is_admin_online")
                {
                    if(is_admin_online == true)
                    {
                        state_return = "IP:"+admin_ip;
                    }
                }
                if(msg == "is_test_online")
                {
                    if(is_test_online == true)
                    {
                        state_return = "IP:"+test_ip;
                    }
                }
                clientSock->write(state_return.toLocal8Bit());//反馈给客户端登录是否成功
                //反馈的信息输出在屏幕上
                qDebug() << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss") << "  "
                         << "服务端ip:" << clientSock->peerAddress().toString()
                         << ",port:"   << QString::number(clientSock->peerPort(), 10)
                         << "发送:" << state_return;
                state_return = "false";//重置登录验证状态
            }
        }
        else
        {
            qDebug() << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss") << "  "
                     << "客户端ip:" << clientSock->peerAddress().toString()
                     << ",port:"   << QString::number(clientSock->peerPort(), 10)
                     << "断开连接";
        }
    }
}

void my_tcp_server::disConnection()
{
    //输出断开信息
    qDebug() << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss") << "  "
             << "客户端ip:" << clientSock->peerAddress().toString()
             << ",port:"   << QString::number(clientSock->peerPort(), 10)
             << "断开连接";
    clientSock->deleteLater();
    clientSock = NULL;
}
