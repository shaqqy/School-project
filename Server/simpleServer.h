#ifndef SIMPLESERVER_H
#define SIMPLESERVER_H

#include <QObject>
#include <QTcpServer>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <vector>
#include <QFile>
#include <QDataStream>


#define MAXCLIENT 2

class Server: public QObject
{
Q_OBJECT
public:
    void operator=(const Server&) = delete;
    Server(Server &other) = delete;
    static Server* GetInstance();
public slots:
  void acceptConnection();
  void startRead();
  void startGame();
  void handleMessage();
  void handleDisconnect();

protected:
  QHostAddress multicastGroup;
  QUdpSocket *multicast;
  QTcpServer *msg_server;
  bool running = false;

private:
  int rdyCounter = 0;
  std::vector<bool> player_alive;
  QList<QTcpSocket*> messengers;
  Server(QObject * parent = 0);
  ~Server();
  static Server* server_;
};
#endif // SIMPLESERVER_H
