#include <QCoreApplication>
#include "simpleServer.h"
#include "simpleClient.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
//    Server server;
//    Client client;
//    client.start("127.0.0.1",8888);
    Server *test = Server::GetInstance();
    test->test = "WOW";
    test = Server::GetInstance();
    qDebug() << test->test;
    return a.exec();
}
