#include "my_tcp_server.h"
#include <iostream>
#include <string>
#include <QString>

using namespace std;

my_tcp_server::my_tcp_server(QObject *parent)
    : QObject( parent )
{
    serverSock = new QTcpServer(this);
    clientSock = NULL;

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
             << "客户端ip:"  << clientSock->localAddress().toString()
             << ",port:"  << QString::number(clientSock->localPort(), 10)
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
                 << "客户端ip:" << clientSock->localAddress().toString()
                 << ",port:"   << QString::number(clientSock->localPort(), 10)
                 << "收到:" << msg;

        if(clientSock != NULL)
        {
            QString login_re = "false";
            if(!login_re.isEmpty())
            {
                if(msg == "admin#admin" || msg == "test#test")//判定用户名密码是否正确
                {
                    login_re = "true";
                }
                clientSock->write(login_re.toLocal8Bit());//反馈给客户端登录是否成功
                //反馈的信息输出在屏幕上
                qDebug() << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss") << "  "
                         << "服务端ip:" << clientSock->localAddress().toString()
                         << ",port:"   << QString::number(clientSock->localPort(), 10)
                         << "发送:" << login_re;
                login_re = "false";//重置登录验证状态
            }
        }
        else
        {
            qDebug() << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss") << "  "
                     << "客户端ip:" << clientSock->localAddress().toString()
                     << ",port:"   << QString::number(clientSock->localPort(), 10)
                     << "断开连接";
        }
    }
}

void my_tcp_server::disConnection()
{
    //输出断开信息
    qDebug() << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss") << "  "
             << "客户端ip:" << clientSock->localAddress().toString()
             << ",port:"   << QString::number(clientSock->localPort(), 10)
             << "断开连接";
    clientSock->deleteLater();
    clientSock = NULL;
}
