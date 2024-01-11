#ifndef SIMPLESERVER_H
#define SIMPLESERVER_H

#include <QObject>

// Darauf achten, dass in der .pro-Datei folgender Eintrag steht
// QT += network
// Falls nachträglich hinzugefügt, dann qmake nocheinmal starten
// sonst werden die beiden folgenden Zeilen nicht erkannt
#include <QTcpServer>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <vector>



#define MAXCLIENT 2

class Server: public QObject
{
Q_OBJECT
public:
  ~Server();
    QString test;
    void operator=(const Server&) = delete;
    Server(Server &other) = delete;
    static Server* GetInstance();
    QUdpSocket *socket;

public slots:
  void acceptConnection();
  void startRead();
  void passMessage(QByteArray msg, QUdpSocket *target = nullptr);
  void decideMessage(QByteArray msg, QHostAddress &sender, quint16 &port);
  void readMoves();

protected:
  QHostAddress player1;
  QHostAddress player2;
  QHostAddress multicastGroup;
  QUdpSocket *multicast;

private:
  std::vector<std::tuple<QHostAddress, quint16>> connectedPlayers;
  void addPlayerToMulticast(QHostAddress player);
  Server(QObject * parent = 0);
    static Server* server_;
};

//Server* Server::server_ = nullptr;
//Server* Server::GetInstance(){
//  if(server_ == nullptr){
//      server_ = new Server();
//  }
//  return server_;
//}
#endif // SIMPLESERVER_H
