#include "network.h"

Network::Network(QObject *parent) : QObject(parent) {
  qDebug() << "[SYS] Initializing networker ...";

  udpSocket = new QUdpSocket(this);
  tcpSocket = new QTcpSocket(this);

  connect(tcpSocket, &QTcpSocket::disconnected, this,
          &Network::tcpDisconnected);

  qDebug() << "[SYS] Initialized networker";
}

void Network::initUdpSocket() {
  connect(udpSocket, &QUdpSocket::readyRead, this, &Network::readNewUdpData);

  /*
   *  Using TCP socket to get the right network interface with IPv4 address
   */
  tcpSocket->connectToHost(QHostAddress("8.8.8.8"), 53);

  if (tcpSocket->waitForConnected(2000)) {
    qDebug() << "[NET] Using IPv4 address: "
             << tcpSocket->localAddress().toString();
  }

  /*
   *  Binding the UDP socket to the right network interface
   */

  udpSocket->bind(tcpSocket->localAddress(), 30000);
  udpSocket->joinMulticastGroup(QHostAddress("239.255.43.21"));
  tcpSocket->close();
}

void Network::readNewUdpData() {
  while (udpSocket->hasPendingDatagrams()) {
    QByteArray buffer;
    buffer.resize(udpSocket->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;

    udpSocket->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);

    qDebug() << "[NET] UDP message from " << sender.toString() << ":"
             << senderPort;
    qDebug() << "[NET] UDP message: " << buffer;
    auto tmp = QString(buffer);
    auto x = tmp.split("x")[0].toInt();
    auto y = tmp.split("x")[1].toInt();
    LOP->setX(x);
    LOP->setY(y);
  }
}

void Network::sendUdpMessage(QByteArray message) {
  udpSocket->writeDatagram(message, QHostAddress("239.255.43.21"), 30001);

  qDebug() << "[NET] Sent message (UDP): " << message;
}

void Network::initTcpSocket() {
  if (tcpSocket->state() != QTcpSocket::ConnectedState &&
      tcpSocket->state() != QTcpSocket::BoundState) {
    connect(tcpSocket, &QTcpSocket::readyRead, this, &Network::readNewTcpData);

    tcpSocket->connectToHost(QHostAddress("192.168.0.2"), 30001);

    if (tcpSocket->waitForConnected(2000)) {
      qDebug() << "[NET] Connected with (TCP): " << tcpSocket->peerAddress();

      emit tcpConnectionStatus(true);
    } else {
      qDebug() << "[NET] Server not reachable (TCP)";

      emit tcpConnectionStatus(false);
    }
  } else {
    qDebug() << "[NET] Already connected via (TCP) to: "
             << tcpSocket->peerAddress();
  }
}

void Network::tcpDisconnected() {
  qDebug() << "[NET] Disconnected from Server (TCP)";

  emit tcpConnectionStatus(false);
}

void Network::readNewTcpData() {
  QByteArray buffer;
  QString prefix;
  buffer.resize(tcpSocket->readBufferSize());

  buffer = tcpSocket->readAll();

  QString message = QString::fromStdString(buffer.toStdString());
  if (message.startsWith("Dead")) {
    emit newChatMessage("The enemy is out", SchoolSkipper::INCOMING_MESSAGE);
    qDebug() << "[NET] TCP message from " << tcpSocket->peerAddress() << ":"
             << tcpSocket->peerPort();
    qDebug() << "[NET] TCP message: " << message;
  } else if (message.startsWith("Start")) {
    emit startFromServer();
    qDebug() << "[NET] TCP message from " << tcpSocket->peerAddress() << ":"
             << tcpSocket->peerPort();
    qDebug() << "[NET] TCP message: " << message;
  }else if(message.startsWith("Coord")){
    message = message.split(" ").at(1);
    auto x = message.split("x")[0].toInt();
    auto y = message.split("x")[1].toInt();
    LOP->setX(x);
    LOP->setY(y);
  } else {
    message = message.split(" ").at(1);

    qDebug() << "[NET] TCP message from " << tcpSocket->peerAddress() << ":"
             << tcpSocket->peerPort();
    qDebug() << "[NET] TCP message: " << message;

    emit newChatMessage(message, SchoolSkipper::INCOMING_MESSAGE);
  }
}

void Network::sendTcpMessage(QByteArray message) {
  tcpSocket->write(message, message.length());

  qDebug() << "[NET] Sent message (TCP): " << message;
}
