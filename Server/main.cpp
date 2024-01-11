#include <QCoreApplication>
#include "simpleServer.h"
//#include "simpleServer.cpp"
#include "stub_client.h"
Server* Server::server_ = NULL;


int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);
    Server *test = Server::GetInstance();
    test->test = "WOW";
    qDebug() << test->socket->socketDescriptor();
    qDebug() << test->socket->peerPort();
    Stub_Client *st_cl = new Stub_Client();
//    test->test = "WOW";
//    test = Server::GetInstance();
//    qDebug() << test->test;
    return a.exec();
}
