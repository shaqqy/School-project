#ifndef NETWORK_H
#define NETWORK_H

#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QNetworkDatagram>
#include <QTextBrowser>
#include <QHostInfo>

#include "enums.h"


class Network : public QObject
{
    Q_OBJECT

public:
    QPointF* COP;
    QPointF* LOP;
    Network(QObject* parent);

    void initUdpSocket();
    void initTcpSocket();

private:
    QUdpSocket* udpSocket;
    QTcpSocket* tcpSocket;

public slots:
    void readNewUdpData();
    void readNewTcpData();

    void sendUdpMessage(QByteArray message);
    void sendTcpMessage(QByteArray message);

    void tcpDisconnected();

signals:
    void newChatMessage(QString message, SchoolSkipper type);
    void tcpConnectionStatus(bool connected);
    void startFromServer();
};

#endif // NETWORK_H
