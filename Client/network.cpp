#include "network.h"

Network::Network(QObject* parent) : QObject(parent)
{
    socket = new QUdpSocket(this);
}

void Network::initSocket(int port) {
    connect(socket, &QUdpSocket::readyRead, this, &Network::read);

    /*
     *  Using TCP socket to get the right network interface with IPv4 address
     */
    QTcpSocket checkSocket;
    checkSocket.connectToHost(QHostAddress("8.8.8.8"), 53);

    if(checkSocket.waitForConnected(2000)){
        qDebug() << "[NET] Using IPv4 address: " << checkSocket.localAddress().toString();
    }

    /*
     *  Binding the UDP socket to the right network interface
     */
    socket->bind(checkSocket.localAddress(), port);
}

void Network::read() {
    while (socket->hasPendingDatagrams()) {
        QByteArray buffer;
        buffer.resize(socket->pendingDatagramSize());

        QHostAddress sender;
        quint16 senderPort;
        socket->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);

        qDebug() << "[NET] Message from " << sender.toString() << ":" << senderPort;
        qDebug() << "[NET] Message: " << buffer;
    }
}

void Network::sendMessage(QByteArray message) {
    socket->writeDatagram(message, QHostAddress("192.168.184.217"), 30001);

    qDebug() << "[NET] Sent message: " << message;
}
