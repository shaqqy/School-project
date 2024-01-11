#include "stub_client.h"
#include "qnetworkdatagram.h"

#include <QHostInfo>

Stub_Client::Stub_Client(QObject *parent)
    : QObject{parent}
{
    sender = new QUdpSocket(this);
    sendDataGram(QHostAddress("10.10.197.31"),30001, "Connect");
    qDebug() << "Looking for all available addresses";
    QList<QHostAddress> list = QHostInfo::fromName(QHostInfo::localHostName()).addresses();
    for (int var = 0; var < list.size(); ++var) {
        qDebug() << list[var];
    }
    sendDataGram(QHostAddress("239.255.43.21"),30000,"200x300");
}

bool Stub_Client::sendDataGram(QHostAddress address, quint16 port, QString payload)
{
    QByteArray buffer;
    QString tmp = payload;
    buffer = tmp.toUtf8();


    QNetworkDatagram *datagram = new QNetworkDatagram(buffer, address, port);
    sender->writeDatagram(*datagram);
    return false;
}
