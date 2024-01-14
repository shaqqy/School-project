#include "simpleServer.h"
#include <QDateTime>
#include <QTcpSocket>


Server::Server(QObject* parent): QObject(parent)
{
    QTcpSocket Socket;
    Socket.connectToHost(QHostAddress("8.8.8.8"),53);
    if(Socket.waitForConnected(2000)){
        qDebug() << Socket.localAddress().toString();
    }
    socket = new QUdpSocket();
    socket->bind(QHostAddress(Socket.localAddress().toString()) /*"192.168.184.217")*/, 30001);
    qDebug() << socket->localAddress().toString();
//    connect(msg_server, &QTcpServer::newConnection, this, &Server::acceptConnection);
//    msg_server->listen(Socket.localAddress(),30002);
    Socket.disconnectFromHost();
    connect(socket, &QAbstractSocket::readyRead, this, &Server::startRead);
    multicastGroup = QHostAddress(QStringLiteral("239.255.43.21"));
    multicast = new QUdpSocket(this);
    multicast->bind(QHostAddress::AnyIPv4, 30000, QUdpSocket::ShareAddress);
    multicast->joinMulticastGroup(multicastGroup);
    connect(multicast, &QAbstractSocket::readyRead, this, &Server::readMoves);
    qDebug() << socket->localAddress();
    qDebug() << multicast->localAddress();
    QString tmp = "Sup?";
    QByteArray t = tmp.toUtf8();
    QHostAddress adrian = QHostAddress("192.168.184.84");
    quint16 port = 1234;
    QNetworkDatagram tt = QNetworkDatagram(t, adrian, port);
    socket->writeDatagram(tt);
}

Server::~Server()
{

}

void Server::acceptConnection()
{
    messengers.append(msg_server->nextPendingConnection());
    connect(messengers.last(), &QTcpSocket::readyRead, this, &Server::handleMessage);
}

void Server::startRead()
{
    QByteArray buffer;
    buffer.resize(socket->pendingDatagramSize());
//    qDebug() << buffer.size();
    QHostAddress sender;
    quint16 senderPort;
    socket->readDatagram(buffer.data(),buffer.size(),&sender,&senderPort);
    qDebug() << "Sender Address: " << sender;
    qDebug() << "Sender Port: " << senderPort;
    qDebug() << "Message: " << buffer;
    decideMessage(buffer, sender, senderPort);
}

void Server::passMessage(QByteArray msg, QUdpSocket *target)
{
    target = nullptr;
}

void Server::decideMessage(QByteArray msg, QHostAddress &sender, quint16 &port)
{
    std::string msg_as_string = msg.toStdString();
    size_t pos_connected = msg_as_string.find("Connect");
    size_t pos_msg = msg_as_string.find("Message");
    size_t pos_dead = msg_as_string.find("Dead");
    if(pos_connected == 0){
        //Add the player to the multicast group and add it to the vector of known players
        auto it = std::find_if(connectedPlayers.begin(), connectedPlayers.end(), [sender](const std::tuple<QHostAddress,quint16>& e) {return std::get<QHostAddress>(e) == sender;});
        if (it != connectedPlayers.end()) {
            //Someone with that IP is already playing
            qDebug() << "Someone already playing with that IP";
        }else{
            //Add the player
            connectedPlayers.push_back(std::make_tuple(sender,port));
            qDebug() << "Added the sender " << sender << " to the connectedPlayers";
            //Send back a multicast address which is to be used for the other players AND the server
            // TODO
            QString multicastAddress = multicastGroup.toString();
            QByteArray writeBuffer = multicastAddress.toUtf8();
            QNetworkDatagram toSend = QNetworkDatagram(writeBuffer,sender,port);
            socket->writeDatagram(toSend);
        }
    }
    if(pos_msg == 0){
        //Player message to send to the other players

    }
    if(pos_dead == 0){
        //Player who didnt send the message has won
    }
}

void Server::readMoves()
{
    QByteArray buffer;
    buffer.resize(multicast->pendingDatagramSize());
    //    qDebug() << buffer.size();
    QHostAddress sender;
    quint16 senderPort;
    multicast->readDatagram(buffer.data(),buffer.size(),&sender,&senderPort);
    qDebug() << "Sender Address: " << sender;
    qDebug() << "Sender Port: " << senderPort;
    qDebug() << "Message: " << buffer;
    if(running){

    }
}

void Server::startGame()
{

    running = true;
    QString tmp = "Start";
    QByteArray buffer;
    buffer = tmp.toUtf8();
    QNetworkDatagram datagram = QNetworkDatagram(buffer,multicastGroup,30000);
    socket->writeDatagram(datagram);
}

void Server::handleMessage()
{
    QByteArray buffer;

}

Server* Server::GetInstance(){
    if(server_ == nullptr){
        server_ = new Server();
    }
    return server_;
}
