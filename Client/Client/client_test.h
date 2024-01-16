#ifndef CLIENT_TEST_H
#define CLIENT_TEST_H

#include <QUdpSocket>
#include <QObject>
#include <QTcpSocket>
#include <QObject>

class Client_test : public QObject
{
    Q_OBJECT
public:
    QTcpSocket *socket;
    QTcpSocket *dafuq;
    QUdpSocket *sender;
    QUdpSocket *receiver;
    explicit Client_test(QObject *parent = nullptr);
    bool sendDataGram(QHostAddress address, quint16 port, QString payload);
    void sendMessage(QString message);
public slots:
    void receiveDatagram();
    void multicastAccepting();
signals:


};

#endif // CLIENT_TEST_H
