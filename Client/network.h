#ifndef NETWORK_H
#define NETWORK_H

#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QNetworkDatagram>
#include <QTextBrowser>
#include <QHostInfo>
#include <QRegularExpression>

class Network : public QObject
{
    Q_OBJECT

public:
    Network(QObject* parent);

    void initUdpSocket(int port);
    void initTcpSocket();

    void initUdpSocket(int port);
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
    void tcpConnected();

signals:
    void chatMessageReadySignal(QString message);
    void chatConnectedStatusSignal(bool connected);
};

#endif // NETWORK_H
