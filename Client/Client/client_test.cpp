#include "client_test.h"

Client_test::Client_test(QObject *parent)
    : QObject{parent}
{
    QTcpSocket socket;
    socket.connectToHost(QHostAddress("8.8.8.8"),53);
    if(socket.waitForConnected(2000)){
        //        qDebug() << socket.localAddress().toString();
    }
    dafuq = new QTcpSocket(this);
    dafuq->connectToHost(socket.localAddress(),30001);
    if(dafuq->waitForConnected(4000)){
        sendMessage("Message WOW");
    }
    receiver = new QUdpSocket(this);
    receiver->bind(QHostAddress::AnyIPv4, 29999, QUdpSocket::ShareAddress);
    receiver->joinMulticastGroup(QHostAddress("239.255.43.21"));
    connect(receiver, &QAbstractSocket::readyRead, this, &Client_test::receiveDatagram);
}

bool Client_test::sendDataGram(QHostAddress address, quint16 port, QString payload)
{
    return true;
}

void Client_test::sendMessage(QString message)
{
    dafuq->write(message.toUtf8());
}

void Client_test::receiveDatagram()
{

}

void Client_test::multicastAccepting()
{

}
