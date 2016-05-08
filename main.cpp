#include <QCoreApplication>
#include "my_tcp_server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    my_tcp_server my_server;

    return a.exec();
}
