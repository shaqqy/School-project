#include "stub_client.h"
#include "qnetworkdatagram.h"

#include <QHostInfo>
#include <QTcpSocket>

Stub_Client::Stub_Client(QObject *parent)
    : QObject{parent}
{
    QTcpSocket socket;
    socket.connectToHost(QHostAddress("8.8.8.8"),53);
    if(socket.waitForConnected(2000)){
//        qDebug() << socket.localAddress().toString();
    }
    sender = new QUdpSocket(this);
    receiver = new QUdpSocket(this);
    receiver->bind(QHostAddress::AnyIPv4, 29999, QUdpSocket::ShareAddress);
    receiver->joinMulticastGroup(QHostAddress("239.255.43.21"));
    connect(receiver, &QAbstractSocket::readyRead, this, &Stub_Client::receiveDatagram);
    sendDataGram(QHostAddress(socket.localAddress().toString()),30001, "Connect");
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

void Stub_Client::receiveDatagram(){
    QByteArray buffer;
    QString tmp;
    QHostAddress sender;
    quint16 senderPort;
    buffer.resize(receiver->pendingDatagramSize());
    receiver->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);
    qDebug() << buffer;
}
