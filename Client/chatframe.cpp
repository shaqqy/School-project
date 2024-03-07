#include "chatframe.h"

ChatFrame::ChatFrame(QWidget* parent) : QFrame(parent){
    qDebug() << "[SYS] Initializing chat frame ...";

    setStyleSheet("background-color: black;");
    initWidgets();

    qDebug() << "[SYS] Initialized chat frame";
}

void ChatFrame::newMessage(QString message, SchoolSkipper type) {
    QString sender;

    switch (type) {
    case SchoolSkipper::INCOMING_MESSAGE:
        sender = "Opponent";
        break;

    case SchoolSkipper::OUTGOING_MESSAGE:
        sender = "You";
        emit sendMessageOverNetwork(QByteArray("Message " + message.toUtf8()));
        break;

    default:
        qDebug() << "[SYS] Got message with invalid message type";
    }

    textBox->setText(textBox->toPlainText() + "[" + QTime::currentTime().toString() + "](" + sender + "): " + message + "\n");
}

void ChatFrame::initWidgets() {
    expandButton = new QPushButton(this);

    connectedStatusLabel = new QLabel(this);
    reconnectButton = new QPushButton(this);
    minimizeButton = new QPushButton(this);

    textBox = new QTextBrowser(this);

    inputBox = new QLineEdit(this);
    sendButton = new QPushButton(this);

    initWidgetsDefaultParams();
    initWidgetsStyles();
}

void ChatFrame::initWidgetsDefaultParams() {
    expandButton->resize(QSize(40, 40));

    inputBox->setPlaceholderText("Type Message ...");
    inputBox->setTextMargins(QMargins(5, 0, 0, 0));
}

void ChatFrame::initWidgetsStyles() {
    expandButton->setStyleSheet("background: white; border-left: 3px solid grey; border-top: 3px solid grey; border-bottom: 3px solid grey; border-top-left-radius: 20px; border-bottom-left-radius: 20px;");

    connectedStatusLabel->setStyleSheet("background-color: black; border-top: 3px solid grey; border-right: 1px solid white;");

    reconnectButton->setStyleSheet("border-top: 3px solid grey; border-right: 3px solid grey; border-top-right-radius: 10px; background-color: black;");
    reconnectButton->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));

    minimizeButton->setStyleSheet("background: black; border-top: 3px solid grey; border-left: 3px solid grey; border-top-left-radius: 10px;");
    minimizeButton->setIcon(QIcon(CHAT_MINIMIZE_ICON));

    textBox->setStyleSheet("background-color: black; border-right: 3px solid grey; border-top: 1px solid white; color: white;");

    inputBox->setStyleSheet("background-color: dark-grey; color: white; border: none; border-top: 1px solid white; border-bottom: 3px solid grey;");

    sendButton->setStyleSheet("border-right: 3px solid grey; border-bottom: 3px solid grey; border-top: 1px solid white; border-bottom-right-radius: 10px; background-color: black;");
    sendButton->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
}

void ChatFrame::paintEvent(QPaintEvent*) {
    const int statusBarItemsCount = 3;
    const int statusBarItemsWidth = width() / statusBarItemsCount;

    const int inputBoxItemsCount = 2;
    const int inputBoxItemsWidth = width() / inputBoxItemsCount;

    connectedStatusLabel->move(0,0);
    connectedStatusLabel->resize(statusBarItemsWidth, (int) SchoolSkipper::CUSTOM_MENU_BAR_HEIGHT);

    minimizeButton->move(statusBarItemsWidth, 0);
    minimizeButton->resize(statusBarItemsWidth, (int) SchoolSkipper::CUSTOM_MENU_BAR_HEIGHT);

    reconnectButton->move(statusBarItemsWidth * 2, 0);
    reconnectButton->resize(statusBarItemsWidth, (int) SchoolSkipper::CUSTOM_MENU_BAR_HEIGHT);

    textBox->move(0, (int) SchoolSkipper::CUSTOM_MENU_BAR_HEIGHT);
    textBox->resize(width(), height() - (int) SchoolSkipper::CUSTOM_MENU_BAR_HEIGHT - (int) SchoolSkipper::CHAT_INPUT_BOX_HEIGHT);

    inputBox->move(0, height() - (int) SchoolSkipper::CHAT_INPUT_BOX_HEIGHT);
    inputBox->resize(inputBoxItemsWidth, (int) SchoolSkipper::CHAT_INPUT_BOX_HEIGHT);

    sendButton->move(width() - inputBoxItemsWidth, height() - (int) SchoolSkipper::CHAT_INPUT_BOX_HEIGHT);
    sendButton->resize(inputBoxItemsWidth, (int) SchoolSkipper::CHAT_INPUT_BOX_HEIGHT);
}
