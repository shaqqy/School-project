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
    void sendMessage(QByteArray message);

private:
    QUdpSocket* udpSocket;
    QTcpSocket* tcpSocket;

public slots:
    void readNewUdpData();
};

#endif // NETWORK_H
