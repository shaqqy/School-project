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

    QPointF *getReceivedCoords() const;
    void setReceivedCoords(QPointF *newReceivedCoords);

private:
    QUdpSocket* udpSocket;
    QTcpSocket* tcpSocket;
    QPointF *receivedCoords;

public slots:
    void readNewUdpData();
    void readNewTcpData();

    void sendUdpMessage(QByteArray message);
    void sendTcpMessage(QByteArray message);
    void sendTcpMessage(QString message, int _type);
};

#endif // NETWORK_H
