#ifndef CHATFRAME_H
#define CHATFRAME_H

#include "QFrame"
#include "QPushButton"
#include "QTextBrowser"
#include "QLineEdit"
#include "QLabel"
#include "QDebug"
#include "QStyle"
#include "QTime"
#include "QScrollBar"

#include "enums.h"


class ChatFrame : public QFrame
{
    Q_OBJECT

public:
    ChatFrame(QWidget* parent);

    const QString CHAT_MINIMIZE_ICON = ":/images/images/chat-close.png";

signals:
    void sendMessageOverNetwork(QByteArray message);

public slots:
    void newMessage(QString message, SchoolSkipper type);

private:
    QPushButton* expandButton;

    QLabel* connectedStatusLabel;
    QPushButton* minimizeButton;
    QPushButton* reconnectButton;

    QTextBrowser* textBox;

    QLineEdit* inputBox;
    QPushButton* sendButton;

    void scrollTextBoxToEnd();

    void initWidgets();
    void initWidgetsDefaultParams();
    void initWidgetsStyles();

protected:
    virtual void paintEvent(QPaintEvent* paint) override;
};

#endif // CHATFRAME_H
