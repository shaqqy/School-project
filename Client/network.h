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
    void initSocket(int port);
    void sendMessage(QByteArray message);

private:
    QUdpSocket* socket;
    QHostAddress* networkInterfaceAddress;

public slots:
    void read();
};

#endif // NETWORK_H
