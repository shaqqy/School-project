#include "mainwindow.h"
#include "ui_mainwindow.h"

SchoolSkipperClient::SchoolSkipperClient(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    isChatWindowInitialized = false;
    isChatVisible = false;
    isGraphicsViewsInitialized = false;

    numberOfPlayers = 3;

    ui->setupUi(this);

    setWindowTitle("School Skipper");
    setWindowIcon(QIcon(":/images/images/icon.png"));
    setStyleSheet("background-color: black;");
    resize(QSize(900, 700));

    networker = new Network(this);
    networker->initTcpSocket();

    /*
     *  Initialization of application components. Chat window needs to be initialized first!
     */
    initChatWindow();
    initGraphicsViews();
}

SchoolSkipperClient::~SchoolSkipperClient()
{
    delete ui;
}

void SchoolSkipperClient::initGraphicsViews() {
    qDebug() << "[SYS] Initializing graphics views ...";

    graphicsViewsGridFrame = new QFrame(this);
    graphicsViewsGridFrame->setStyleSheet("border-top-left-radius: 10px; border-bottom-left-radius: 10px; border: 3px solid grey;");
    graphicsViewsGridFrame->stackUnder(chatExpandButton);

    graphicsViewsGrid = new QGridLayout(graphicsViewsGridFrame);

    graphicsScene = new QGraphicsScene();

    for (int column = 0; column < numberOfPlayers / 2; column++) {
        for (int row = 0; row < 2; row++) {
            QGraphicsView* currentGraphicsView = new QGraphicsView(graphicsScene, graphicsViewsGridFrame);
            currentGraphicsView->setStyleSheet("border: 1px solid white;");

            graphicsViewsGrid->addWidget(currentGraphicsView, row, column);

            graphicsViewsList.append(currentGraphicsView);
        }
    }

    isGraphicsViewsInitialized = true;

    qDebug() << "[SYS] Initialized graphics views";
}

void SchoolSkipperClient::initChatWindow() {
    qDebug() << "[SYS] Initializing chat window ...";

    chatWindow = new QTextBrowser(this);
    chatWindow->setStyleSheet("background-color: black; border-right: 3px solid grey; border-top: 1px solid white; color: white;");

    chatConnectedStatus = new QLabel(this);
    chatConnectedStatus->setAlignment(Qt::AlignCenter);
    chatConnectedStatus->setStyleSheet("background-color: black; border-top: 3px solid grey; border-right: 1px solid white;");

    sendButton = new QPushButton(this);
    sendButton->setStyleSheet("border-right: 3px solid grey; border-bottom: 3px solid grey; border-top: 1px solid white; border-bottom-right-radius: 10px; background-color: black;");
    sendButton->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
    sendButton->show();

    reconnectButton = new QPushButton(this);
    reconnectButton->setStyleSheet("border-top: 3px solid grey; border-right: 3px solid grey; border-top-right-radius: 10px; background-color: black;");
    reconnectButton->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));

    chatExpandButton = new QPushButton(this);
    chatExpandButton->resize(QSize(40, 40));
    chatExpandButton->setStyleSheet("background: white; border-left: 3px solid grey; border-top: 3px solid grey; border-bottom: 3px solid grey; border-top-left-radius: 20px; border-bottom-left-radius: 20px;");
    chatExpandButton->setIcon(QIcon(":/images/images/chat.png"));

    chatMinimizeButton = new QPushButton(this);
    chatMinimizeButton->setStyleSheet("background: black; border-top: 3px solid grey; border-left: 3px solid grey; border-top-left-radius: 10px;");
    chatMinimizeButton->setIcon(QIcon(":/images/images/chat-close.png"));

    chatBox = new QLineEdit(this);
    chatBox->setPlaceholderText("Type Message ...");
    chatBox->setTextMargins(QMargins(5, 0, 0, 0));
    chatBox->setStyleSheet("background-color: dark-grey; color: white; border: none; border-top: 1px solid white; border-bottom: 3px solid grey;");

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
    connect(networker, &Network::chatConnectedStatusSignal, this, &SchoolSkipperClient::chatConnectedStatusSlot);
    connect(reconnectButton, &QPushButton::clicked, networker, &Network::initTcpSocket);

    /*
     *  connect for the chat button to display or hide the chat window
     */
    connect(chatExpandButton, &QPushButton::clicked, this, &SchoolSkipperClient::handleVisibilityOfChat);
    connect(chatMinimizeButton, &QPushButton::clicked, this, &SchoolSkipperClient::handleVisibilityOfChat);

    isChatWindowInitialized = true;

    qDebug() << "[SYS] Initialized chat window";
}

void SchoolSkipperClient::handleVisibilityOfChat() {
    if (isChatVisible) {
        isChatVisible = false;

        resize(QSize(this->width() - (int) SchoolSkipper::CHAT_WINDOW_WIDTH, this->height()));
    } else {
        isChatVisible = true;

        resize(QSize(this->width() + (int) SchoolSkipper::CHAT_WINDOW_WIDTH, this->height()));
    }
}

void SchoolSkipperClient::scrollToEnd(QScrollBar* scrollBar) {
    scrollBar->setValue(scrollBar->maximum());

    qDebug() << "[SYS] Changed scroll bar position to: " << scrollBar->maximum();
}

void SchoolSkipperClient::messageReadyToSendSlot() {
    QString message = chatBox->text();

    if (message.length() == 0) {
        return;
    }

    emit messageReadyToSendSignal(QByteArray::fromStdString(message.toStdString()));

    chatBox->setText("");

    chatWindow->setText(chatWindow->toPlainText() + "[" + QTime::currentTime().toString() + "](You): " + message + "\n");

    scrollToEnd(chatWindow->verticalScrollBar());
}

void SchoolSkipperClient::chatMessageReadySlot(QString message) {
    QScrollBar* scrollBar = chatWindow->verticalScrollBar();
    bool scrollBarAtEnd = scrollBar->maximum() - scrollBar->value() <= 10;

    chatWindow->setText(chatWindow->toPlainText() + "[" + QTime::currentTime().toString() + "](Opponent): " + message + "\n");

    if (scrollBarAtEnd) {
        scrollToEnd(scrollBar);
    }
}

void SchoolSkipperClient::chatConnectedStatusSlot(bool connected) {
    isChatConnected = connected;
    repaint();
}

void SchoolSkipperClient::paintEvent(QPaintEvent*) {
    const int width = ui->centralwidget->width();
    const int height = ui->centralwidget->height();

    /*
     *  Position and size calculation for the chat window items
     */
    if (isChatWindowInitialized) {
        if (isChatVisible) {
            const int x = width - (int) SchoolSkipper::CHAT_WINDOW_WIDTH;

            if (isChatConnected) {
                chatConnectedStatus->setStyleSheet("background-color: black; border-top: 3px solid grey; border-right: 1px solid white; color: green;");
                chatConnectedStatus->setText("Connected");
            } else {
                chatConnectedStatus->setStyleSheet("background-color: black; border-top: 3px solid grey; border-right: 1px solid white; color: red;");
                chatConnectedStatus->setText("Disconnected");
            }

            chatWindow->resize((int) SchoolSkipper::CHAT_WINDOW_WIDTH, height - 70);
            chatWindow->move(QPoint(x, 30));

            chatConnectedStatus->resize(QSize(chatWindow->width() / 3 + 9, 30));
            chatConnectedStatus->move(QPoint(x, 0));

            reconnectButton->resize(QSize(chatWindow->width() / 3, 30));
            reconnectButton->move(QPoint(x + (chatWindow->width() - chatWindow->width() / 3), 0));

            chatMinimizeButton->resize(QSize(chatWindow->width() / 3, 30));
            chatMinimizeButton->move(QPoint(x + (chatWindow->width() - ((chatWindow->width() / 3) * 2)), 0));

            sendButton->resize(QSize(chatWindow->width() / 4, 40));
            sendButton->move(QPoint(x + (chatWindow->width() - chatWindow->width() / 4), chatWindow->height() + 30));

            chatBox->resize(QSize(chatWindow->width() - (chatWindow->width() / 4), 40));
            chatBox->move(QPoint(x, chatWindow->height() + 30));

            chatExpandButton->hide();

            chatConnectedStatus->show();
            chatMinimizeButton->show();
            reconnectButton->show();
            sendButton->show();
            chatBox->show();
            chatWindow->show();
        } else {
            chatExpandButton->move(QPoint(width - chatExpandButton->width() + 3, 40));
            chatExpandButton->show();

            chatWindow->hide();
            chatMinimizeButton->hide();
            chatConnectedStatus->hide();
            reconnectButton->hide();
            sendButton->hide();
            chatBox->hide();
        }
    }

    /*
     *  Size calculation for graphics view grid
     */
    if (isGraphicsViewsInitialized) {
        if (isChatVisible) {
            graphicsViewsGridFrame->resize(QSize(width - (int) SchoolSkipper::CHAT_WINDOW_WIDTH, height));
            graphicsViewsGridFrame->setStyleSheet("border-top-left-radius: 10px; border-bottom-left-radius: 10px; border: 3px solid grey;");
        } else {
            graphicsViewsGridFrame->resize(QSize(width, height));
            graphicsViewsGridFrame->setStyleSheet("border-radius: 10px; border: 3px solid grey;");
        }

        graphicsViewsGridFrame->show();
    }
}
