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
#include <QFile>



#define MAXCLIENT 2

class Server: public QObject
{
Q_OBJECT
public:
  ~Server();
    void operator=(const Server&) = delete;
    Server(Server &other) = delete;
    static Server* GetInstance();


public slots:
  void acceptConnection();
  void startRead();
  void passMessage(QByteArray msg, QUdpSocket *target = nullptr);
  void decideMessage(QByteArray msg, QHostAddress &sender, quint16 &port);
  void readMoves();
  void startGame();
  void handleMessage();

protected:
  QHostAddress multicastGroup;
  QUdpSocket *multicast;
  QTcpServer *msg_server;
  bool running = false;
  QFile *log;


private:
  std::vector<std::tuple<QHostAddress, quint16>> connectedPlayers;
  QList<QList<QString>> movesByPlayer;
  QList<QTcpSocket*> messengers;
  void addPlayerToMulticast(QHostAddress player);
  Server(QObject * parent = 0);
  static Server* server_;
};
#endif // SIMPLESERVER_H
