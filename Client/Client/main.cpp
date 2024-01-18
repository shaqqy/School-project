#include <QCoreApplication>
#include "client_test.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Client_test *test = new Client_test();
    return a.exec();
}
