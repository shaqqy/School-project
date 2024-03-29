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

    const QIcon CHAT_MINIMIZE_ICON = QIcon(":/images/images/chat-close.png");
    const QIcon CHAT_EXPAND_ICON_DEFAULT = QIcon(":/images/images/chat.png");
    const QIcon CHAT_EXPAND_ICON_NEW_MESSAGE = QIcon(":/images/images/chat_message_new.png");

    QLineEdit *getInputBox() const;
    void setInputBox(QLineEdit *newInputBox);

signals:
    void minimizeChatFrame();
    void changeIconOfExpandButton(QIcon icon);

    void reconnectChat();

    void newMessage(QString message, SchoolSkipper type);
    void sendMessageOverNetwork(QByteArray message);

public slots:
    void handleNewMessage(QString message, SchoolSkipper type);
    void handleConnectionChange(bool connected);
    void handleMinimizePressed();
    void handleReconnectChat();

    void sendMessagePressed();

private:
    QLabel* connectedStatusLabel;
    QPushButton* minimizeButton;
    QPushButton* reconnectButton;

    QTextBrowser* textBox;

    QLineEdit* inputBox;
    QPushButton* sendButton;

    bool connected;

    void scrollTextBoxToEnd();

    void initWidgets();
    void initWidgetsDefaultParams();
    void initWidgetsStyles();

protected:
    virtual void paintEvent(QPaintEvent* paint) override;
};

#endif // CHATFRAME_H
