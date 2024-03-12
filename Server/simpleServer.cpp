#include "simpleServer.h"
#include <QDateTime>
#include <QTcpSocket>

/*!
 *  \brief Will create the an instance of our server class
 *
 *  Will create a multicast socket and a tcp socket for reliable messaging.
 *  This is a private function and will be called only by GetInstance if the
 * variable server_ is a nullpointer. Furthermore we setup the signals and slots
 * of our tcp server for incoming connections Accepts an QObject as parent for
 * Garbage collection purposes, but that is optional
 */
Server::Server(QObject *parent) : QObject(parent) {
  QTcpSocket Socket;
  Socket.connectToHost(QHostAddress("8.8.8.8"), 53);
  if (Socket.waitForConnected(2000)) {
    qDebug() << Socket.localAddress().toString();
  }
  msg_server = new QTcpServer();
  connect(msg_server, &QTcpServer::newConnection, this,
          &Server::acceptConnection);
  msg_server->listen(/*Socket.localAddress()*/ QHostAddress("192.168.0.2"),
                     30001);
  Socket.disconnectFromHost();
  multicastGroup = QHostAddress(QStringLiteral("239.255.43.21"));
  multicast = new QUdpSocket(this);
  multicast->bind(QHostAddress("192.168.0.2"), 30000, QUdpSocket::ShareAddress);
  multicast->joinMulticastGroup(multicastGroup);
  connect(multicast, &QAbstractSocket::readyRead, this, &Server::startRead);
  qDebug() << multicast->localAddress();
}
/*!
 * \brief Server::~Server Destructor
 *
 * Not really necessary to override anything here as QT will delete
 * automatically everything in relation to an object, if the parent is deleted
 */
Server::~Server() {}

/*!
 * \brief Server::acceptConnection accepts incoming tcp connection requests
 *
 *  Will accept an incoming connection and connect the readyRead signal to our
 message handling slot. Further we connect the disconnected signal to our
 handleDisconnect slot.
 *  Will also add the socket to the messengers list and add an entry to the
 player_alive vector
 */
void Server::acceptConnection() {
  QTcpSocket *socket = msg_server->nextPendingConnection();
  messengers.append(socket);

  connect(socket, &QTcpSocket::readyRead, this, &Server::handleMessage);
  connect(socket, &QTcpSocket::disconnected, this, &Server::handleDisconnect);
  qDebug() << socket->peerAddress().toString() + ":" +
                  socket->localAddress().toString();
  qDebug() << "Connected player with IP "
           << messengers.last()->peerAddress().toString();
  player_alive.push_back(true);
}
/*!
 * \brief Server::startRead
 *
 * Will read incoming Datagrams and will print them to the console with sender
 * information Used for debugging purposes
 */
void Server::startRead() {
//  QByteArray buffer;
//  buffer.resize(multicast->pendingDatagramSize());
//  //    qDebug() << buffer.size();
//  QHostAddress sender;
//  quint16 senderPort;
//  multicast->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);
//  qDebug() << "Sender Address: " << sender;
//  qDebug() << "Sender Port: " << senderPort;
//  qDebug() << "Message: " << buffer;
//  if(log->open(QIODevice::ReadWrite)){
//      QTextStream stream(log);
//      stream <<sender.toString() << "," <<buffer << Qt::endl;
//  }
  QNetworkDatagram t = multicast->receiveDatagram();
  qDebug() << t.data();
}

/*!
 * \brief Server::startGame will start the game once all players are ready
 *
 * Is called always when a player sends a ready message, if all players are
 * ready it'll start the game by setting running to true and sending the players
 * the start message
 */
void Server::startGame() {
  if (messengers.length() == 2) {
        QDateTime date = QDateTime::currentDateTime();
        QString formattedTime = date.toString("dd.MM.yyyy hh:mm:ss");
        QByteArray formattedTimeMsg = formattedTime.toLocal8Bit();
        log = new QFile(formattedTimeMsg+".csv");
        if(log->open(QIODevice::ReadWrite)){
            QTextStream stream(log);
            stream << "START" << Qt::endl;
        }
    for (int i = 0; i < messengers.length(); i++) {
      if (player_alive[i] == false) {
        return;
      }
    }
    qDebug() << "Started Game";
    running = true;
    QString tmp = "Start";
    QByteArray buffer;
    buffer = tmp.toUtf8();
    QNetworkDatagram datagram = QNetworkDatagram(buffer, multicastGroup, 30000);
    multicast->writeDatagram(datagram);
    foreach(const auto& mg, messengers){
      mg->write(buffer);
    }
  }
}

/*!
 * \brief Server::handleMessage Checks for keywords and acts accordingly
 *
 * Checks for the Dead, Message and Ready Keywords, will mark a player as dead,
 * will send the message to the other player or will check if we can start the
 * game
 */
void Server::handleMessage() {
  QByteArray buffer;
  QTcpSocket *sender = (QTcpSocket *)QObject::sender();
  buffer.resize(sender->readBufferSize());
  buffer = sender->readAll();
  //    buffer = sender->read(sender->readBufferSize());
  qDebug() << buffer;
  QString msg = QString(buffer);
  //    for(int i = 0; i < messengers.length(); i++)
  //    {
  //        if(messengers[i]->peerAddress() != sender->peerAddress()){
  //            serverSender(messengers[i], msg);
  //            qDebug() << "Sent message";
  //        }
  //    }
  //    qDebug() << msg;
  if (msg.startsWith("Dead")) {
    qDebug() << "Player with IP " << sender->localAddress() << " died";
      if(log->open(QIODevice::ReadWrite)){
          QTextStream stream(log);
          stream << sender->localAddress().toString() << "," << buffer << Qt::endl;
          stream << "GAME OVER" << Qt::endl;
      }
      foreach (auto const &mg, messengers) {
          if (mg != sender) {
        qDebug() << "Inside the loop message: " + buffer;
        mg->write(buffer, buffer.length());
          }
      }
  } else if (msg.startsWith("Message")) {
    foreach (auto const &mg, messengers) {
      if (mg != sender) {
        qDebug() << "Inside the loop message: " + buffer;
        mg->write(buffer, buffer.length());
      }
    }
  } else if (msg.startsWith("Ready")) {
    if (rdyCounter == 0) {
      // int idx = messengers.indexOf(sender);
      // player_alive[idx] = true;
      rdyCounter++;
      qDebug() << rdyCounter;
    } else if (rdyCounter == 1) {
      startGame();
      qDebug() << "Called startgame";
      rdyCounter++;
    } else {
      rdyCounter = 1;
    }
  }else{
    foreach (auto const &mg, messengers) {
      if (mg != sender) {
        qDebug() << "Inside the loop message: " + buffer;
        mg->write(buffer, buffer.length());
      }
    }
  }
}
/*!
 * \brief Server::handleDisconnect tells the other player he's won
 *
 * If the stable tcp connection between player and server disconnects, we assume
 * the player lost internet connection or something like that and end the game
 * deciding the other player won
 */
void Server::handleDisconnect() {
  QTcpSocket *socket = (QTcpSocket *)QObject::sender();
  if (running) {
    for (int i = 0; i < messengers.length(); i++) {
      if (messengers.at(i) != socket) {
        messengers.at(i)->write("You won!");
      }
    }
  }
  if (messengers.contains(socket)) {
    int i = messengers.indexOf(socket);
    messengers.removeAt(i);
  }
  qDebug() << "Player with IP " << socket->peerAddress() << " disconencted";
  socket->deleteLater();
}
/*!
 * \brief Server::GetInstance Is the function to be used instead of a
 * constructor
 *
 * Will return a pointer to the instance of our server a.k.a Singleton Design
 * Pattern \return Server*
 */
Server *Server::GetInstance() {
  if (server_ == nullptr) {
    server_ = new Server();
  }
  return server_;
}
