#include "simpleServer.h"
#include <QDateTime>


Server::Server(QObject* parent): QObject(parent)
{
    socket = new QUdpSocket();
    socket->bind(QHostAddress("10.10.197.31"), 30001);
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
    QHostAddress adrian = QHostAddress("10.10.182.11");
    quint16 port = 1234;
    QNetworkDatagram tt = QNetworkDatagram(t, adrian, port);
    socket->writeDatagram(tt);
}

Server::~Server()
{

}

void Server::acceptConnection()
{
//    qDebug()
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
    size_t pos_move = msg_as_string.find("Move");
    size_t pos_dead = msg_as_string.find("Dead");
    if(pos_connected == 0){
        //Add the player to the multicast group and add it to the vector of known players
        auto it = std::find_if(connectedPlayers.begin(), connectedPlayers.end(), [sender](const std::tuple<QHostAddress,quint16>& e) {return std::get<QHostAddress>(e) == sender;});
        if (it != connectedPlayers.end()) {
            //Someone with that IP is already playing
            qDebug() << "Found";
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
    if(pos_move == 0){
        //Save the move the player made to the list of made moves or whatever
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
}

Server* Server::GetInstance(){
    if(server_ == nullptr){
        server_ = new Server();
    }
    return server_;
}
