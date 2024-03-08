#include "mainwindow.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << "[SYS] Starting application";

    SchoolSkipperClient schoolSkipperClient;
    schoolSkipperClient.show();

    qDebug() << "[SYS] Started application";

    return a.exec();
}
