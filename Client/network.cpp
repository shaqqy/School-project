#include "network.h"

Network::Network(QObject* parent) : QObject(parent)
{
    udpSocket = new QUdpSocket(this);
    tcpSocket = new QTcpSocket(this);
}

void Network::initUdpSocket(int port) {
    connect(udpSocket, &QUdpSocket::readyRead, this, &Network::readNewUdpData);

    /*
     *  Using TCP socket to get the right network interface with IPv4 address
     */
    tcpSocket->connectToHost(QHostAddress("8.8.8.8"), 53);

    if(tcpSocket->waitForConnected(2000)){
        qDebug() << "[NET] Using IPv4 address: " << tcpSocket->localAddress().toString();
    }

    /*
     *  Binding the UDP socket to the right network interface
     */
    udpSocket->bind(tcpSocket->localAddress(), port);
    tcpSocket->close();
}

void Network::readNewUdpData() {
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray buffer;
        buffer.resize(udpSocket->pendingDatagramSize());

        QHostAddress sender;
        quint16 senderPort;
        udpSocket->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);

        qDebug() << "[NET] Message from " << sender.toString() << ":" << senderPort;
        qDebug() << "[NET] Message: " << buffer;
    }
}

void Network::sendUdpMessage(QByteArray message) {
    udpSocket->writeDatagram(message, QHostAddress("10.10.197.12"), 30001);

    qDebug() << "[NET] Sent message: " << message;
}