#include "mainwindow.h"
#include "ui_mainwindow.h"

SchoolSkipperClient::SchoolSkipperClient(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("School Skipper");
    setWindowIcon(QIcon(":/images/images/icon.png"));
    resize(QSize(900, 700));

    networker = new Network(this);
    networker->initTcpSocket();

    initGameFrames();
    initChatWindow();
    initGraphicsViewAndScene();
}

SchoolSkipperClient::~SchoolSkipperClient()
{
    delete ui;
}

void SchoolSkipperClient::initGameFrames() {
    mainFrame = new CustomFrame(QPixmap(":/images/images/default_background_menu.png"), this);
    mainFrame->setStyleSheet("border-top: 3px solid grey; border-bottom: 3px solid grey; border-left: 3px solid grey; border-right: 3px solid grey;");
    mainFrame->show();

    opponentFrame = new CustomFrame(QPixmap(":/images/images/default_background.png"), this);
    opponentFrame->setStyleSheet("border-top: 3px solid grey; border-bottom: 3px solid grey; border-left: 3px solid grey;");
    opponentFrame->openMainMenu();
    isMenuActive = true;
    opponentFrame->show();
}

void SchoolSkipperClient::initChatWindow() {
    isChatVisible = false;

    chatWindow = new QTextBrowser(this);
    chatWindow->setStyleSheet("background-color: black; border-right: 3px solid grey; border-top: 1px solid white; border-left: 3px solid grey");
    chatWindow->setTextColor(Qt::white);

    chatConnectionStatus = new QLabel(this);
    chatConnectionStatus->setStyleSheet("background-color: black; border-top: 3px solid grey; border-right: 1px solid white; border-left: 3px solid grey; color: white;");
    chatConnectionStatus->setText("Disconnected");

    sendButton = new QPushButton(this);
    sendButton->setStyleSheet("border-right: 3px solid grey; border-bottom: 3px solid grey; border-top: 1px solid white; border-bottom-right-radius: 10px; background-color: black;");
    sendButton->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
    sendButton->show();

    reconnectButton = new QPushButton(this);
    reconnectButton->setStyleSheet("border-top: 3px solid grey; border-right: 3px solid grey; border-top-right-radius: 10px; background-color: black;");
    reconnectButton->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));

    chatButton = new QPushButton(this);
    chatButton->resize(QSize(40, 40));
    chatButton->setStyleSheet("background: white; border-left: 3px solid grey; border-top: 3px solid grey; border-bottom: 3px solid grey; border-top-left-radius: 20px; border-bottom-left-radius: 20px;");
    chatButton->setIcon(QIcon(":/images/images/chat.png"));

    chatBox = new QLineEdit(this);
    chatBox->setPlaceholderText("Type Message ...");
    chatBox->setTextMargins(QMargins(5, 0, 0, 0));
    chatBox->setStyleSheet("background-color: dark-grey; color: white; border: none; border-top: 1px solid white; border-bottom: 3px solid grey; border-left: 3px solid grey;");

    /*
     *  connections to notify when user want's to send a message
     */
    connect(sendButton, &QPushButton::clicked, this, &SchoolSkipperClient::messageReadyToSendSlot);
    connect(chatBox, &QLineEdit::returnPressed, this, &SchoolSkipperClient::messageReadyToSendSlot);

    /*
     *  connections to trigger sending and receving of messages on the network class
     */
    connect(this, &SchoolSkipperClient::messageReadyToSendSignal, networker, &Network::sendTcpMessage);
    connect(networker, &Network::chatMessageReadySignal, this, &SchoolSkipperClient::chatMessageReadySlot);

    /*
     *  connect for updating the connection status or to reconnect
     */
    connect(networker, &Network::chatConnectionStatusSignal, this, &SchoolSkipperClient::chatConnectionStatusSlot);
    connect(reconnectButton, &QPushButton::clicked, networker, &Network::initTcpSocket);

    /*
     *  connect for the chat button to display or hide the chat window
     */
    connect(chatButton, &QPushButton::clicked, this, &SchoolSkipperClient::handleVisibilityOfChat);

    chatBox->show();
}

void SchoolSkipperClient::initGraphicsViewAndScene() {
    graphicsScene = new QGraphicsScene();
    graphicsSceneBackground = graphicsScene->addPixmap(QPixmap(":/images/images/default_background.png"));

    /*
     *  Main Frame graphics view
     */
    graphicsViewMainFrame = new QGraphicsView(graphicsScene, mainFrame);
    graphicsViewMainFrame->centerOn(graphicsSceneBackground->pos());
    graphicsViewMainFrame->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsViewMainFrame->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    graphicsViewMainFrame->show();

    /*
     *  Opponent graphics view
     */
    graphicsViewOpponentFrame = new QGraphicsView(graphicsScene, opponentFrame);
    graphicsViewOpponentFrame->centerOn(graphicsSceneBackground->pos());
    graphicsViewOpponentFrame->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsViewOpponentFrame->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    graphicsViewOpponentFrame->show();
}

void SchoolSkipperClient::handleVisibilityOfChat() {
    if (isChatVisible) {
        isChatVisible = false;
    } else {
        isChatVisible = true;

        resize(QSize(this->width() + 200, this->height()));
    }
}

void SchoolSkipperClient::messageReadyToSendSlot() {
    QString message = chatBox->text();

    if (message.length() == 0) {
        return;
    }

    emit messageReadyToSendSignal(QByteArray::fromStdString(message.toStdString()));

    chatBox->setText("");

    QScrollBar* scrollBar = chatWindow->verticalScrollBar();
    bool scrollBarAtEnd = scrollBar->maximum() - scrollBar->value() <= 10;

    chatWindow->setText(chatWindow->toPlainText() + "\n[" + QTime::currentTime().toString() + "](You): " + message);

    if (scrollBarAtEnd) {
        scrollBar->setValue(scrollBar->maximum());

        qDebug() << "[SYS] Changed scroll bar position to: " << scrollBar->maximum();
    }
}

void SchoolSkipperClient::chatMessageReadySlot(QString message) {
    QScrollBar* scrollBar = chatWindow->verticalScrollBar();
    bool scrollBarAtEnd = scrollBar->maximum() - scrollBar->value() <= 10;

    chatWindow->setText(chatWindow->toPlainText() + "\n[" + QTime::currentTime().toString() + "](Opponent): " + message);

    if (scrollBarAtEnd) {
        scrollBar->setValue(scrollBar->maximum());

        qDebug() << "[SYS] Changed scroll bar position to: " << scrollBar->maximum();
    }
}

void SchoolSkipperClient::chatConnectionStatusSlot(bool connected) {
    if (connected) {
        chatConnectionStatus->setText("Connected");
    } else {
        chatConnectionStatus->setText("Disconnected");
    }
}

void SchoolSkipperClient::paintEvent(QPaintEvent*) {
    const int width = ui->centralwidget->width();
    const int height = ui->centralwidget->height();

    const int gameFramesEndCoordinates = mainFrame->width() + opponentFrame->width();

    setMinimumWidth(gameFramesEndCoordinates + 5);

    /*
     *  Position calculation for main and opponent frame
     */
    opponentFrame->move(QPoint(3, 0));
    mainFrame->move(QPoint(opponentFrame->width() + 3, 0));

    /*
     *  Size calculation for graphics view and scene background
     */
    if (isMenuActive) {
        graphicsSceneBackground->setPixmap(QPixmap(":/images/images/default_background_menu.png").scaled(mainFrame->width(), mainFrame->height()));
    } else {
        graphicsSceneBackground->setPixmap(QPixmap(":/images/images/default_background.png").scaled(mainFrame->width(), mainFrame->height()));
    }
    graphicsViewMainFrame->resize(QSize(mainFrame->width(), mainFrame->height()));
    graphicsViewOpponentFrame->resize(QSize(opponentFrame->width() - 1, opponentFrame->height()));

    /*
     *  Position and size calculation for the chat window items
     */
    if (isChatVisible) {
        chatWindow->resize(width - (gameFramesEndCoordinates + 4), height - 70);
        chatWindow->move(QPoint(gameFramesEndCoordinates, 30));
        chatWindow->show();

        if (chatButton != nullptr) {
            chatButton->hide();
        }

        if (chatConnectionStatus != nullptr) {
            chatConnectionStatus->resize(QSize(chatWindow->width() - (chatWindow->width() / 4), 30));
            chatConnectionStatus->move(QPoint(gameFramesEndCoordinates, 0));

            if (chatConnectionStatus->isHidden()) {
                chatConnectionStatus->show();
            }
        }

        if (reconnectButton != nullptr) {
            reconnectButton->resize(QSize(chatWindow->width() / 4, 30));
            reconnectButton->move(QPoint(gameFramesEndCoordinates + (chatWindow->width() - chatWindow->width() / 4), 0));

            if (reconnectButton->isHidden()) {
                reconnectButton->show();
            }
        }

        if (sendButton != nullptr) {
            sendButton->resize(QSize(chatWindow->width() / 4, 40));
            sendButton->move(QPoint(gameFramesEndCoordinates + (chatWindow->width() - chatWindow->width() / 4), chatWindow->height() + 30));

            if (sendButton->isHidden()) {
                sendButton->show();
            }
        }

        if (chatBox != nullptr) {
            chatBox->resize(QSize(chatWindow->width() - (chatWindow->width() / 4), 40));
            chatBox->move(QPoint(gameFramesEndCoordinates, chatWindow->height() + 30));

            if (chatBox->isHidden()) {
                chatBox->show();
            }
        }
    } else if (!chatWindow->isHidden()) {
        if (chatButton != nullptr) {
            chatButton->move(QPoint(width - chatButton->width() + 3, 40));
            chatButton->show();
        }

        chatConnectionStatus->hide();
        reconnectButton->hide();
        chatWindow->hide();
        sendButton->hide();
        chatBox->hide();
    }
}
