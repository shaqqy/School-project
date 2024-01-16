#include <QCoreApplication>
#include "simpleServer.h"
//#include "simpleServer.cpp"
Server* Server::server_ = NULL;


int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);
    Server *test = Server::GetInstance();

    return a.exec();
}
