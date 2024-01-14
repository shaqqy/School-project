#ifndef STUB_CLIENT_H
#define STUB_CLIENT_H


#include <QHostAddress>
#include <QUdpSocket>
#include <QObject>

class Stub_Client : public QObject
{
    Q_OBJECT
public:
    QUdpSocket *sender;
    QUdpSocket *receiver;
    explicit Stub_Client(QObject *parent = nullptr);
    bool sendDataGram(QHostAddress address, quint16 port, QString payload);

public slots:
    void receiveDatagram();
    void multicastAccepting();
signals:

};

#endif // STUB_CLIENT_H
