#ifndef NETWORK_H
#define NETWORK_H

#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QNetworkDatagram>
#include <QTextBrowser>
#include <QHostInfo>
#include <QRegularExpression>

#include "enums.h"


class Network : public QObject
{
    Q_OBJECT

public:
    Network(QObject* parent);

    void initUdpSocket();
    void initTcpSocket();

    std::vector<QPointF*> L_O_P;
    void saveLastOpponentPosition(QString message);
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
};

#endif // NETWORK_H
