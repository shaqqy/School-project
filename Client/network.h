#ifndef NETWORK_H
#define NETWORK_H

#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QNetworkDatagram>
#include <QTextBrowser>
#include <QHostInfo>

class Network : public QObject
{
    Q_OBJECT

public:
    Network(QObject* parent);

    void initUdpSocket(int port);
    void initTcpSocket(int port);

private:
    QUdpSocket* udpSocket;
    QTcpSocket* tcpSocket;

public slots:
    void readNewUdpData();
    void readNewTcpData();

    void sendUdpMessage(QByteArray message);
    void sendTcpMessage(QByteArray message);
};

#endif // NETWORK_H
