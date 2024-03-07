#include "mainwindow.h"
#include "ui_mainwindow.h"

SchoolSkipperClient::SchoolSkipperClient(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    isMenuBarInitialized = false;
    isChatWindowInitialized = false;
    isGraphicsViewsInitialized = false;
    isChatVisible = false;
    isNewChatMessageAvailable = false;

    numberOfPlayers = 3;

    ui->setupUi(this);

    setWindowTitle("School Skipper");
    setWindowIcon(QIcon(":/images/images/icon.png"));
    setStyleSheet("background-color: white;");
    resize(QSize(350 * numberOfPlayers, 700));

    networker = new Network(this);
    networker->initTcpSocket();

    /*
     *  Initialization of application components. Chat window needs to be initialized first!
     */
    initMenuBar();
    initChatWindow();
    initGraphicsViews();
}

SchoolSkipperClient::~SchoolSkipperClient()
{
    delete ui;
}

void SchoolSkipperClient::initMenuBar() {
    qDebug() << "[SYS] Initializing menu bar ...";

    menuBarFrame = new QFrame(this);
    menuBarFrame->move(QPoint(0, 0));
    menuBarFrame->setStyleSheet("background-color: black; border-top: 3px solid grey; border-left: 3px solid grey; border-top-left-radius: 10px;");

    isMenuBarInitialized = true;

    qDebug() << "[SYS] Initialized menu bar";
}

void SchoolSkipperClient::initGraphicsViews() {
    qDebug() << "[SYS] Initializing graphics views ...";

    graphicsViewsGridFrame = new QFrame(this);
    graphicsViewsGridFrame->move(QPoint(0, (int) SchoolSkipper::CUSTOM_MENU_BAR_HEIGHT));
    graphicsViewsGridFrame->setStyleSheet("background-color: black; border-bottom-left-radius: 10px; border: 3px solid grey; border-top: 1px solid white;");
    graphicsViewsGridFrame->stackUnder(chatExpandButton);

    graphicsViewsGrid = new QGridLayout(graphicsViewsGridFrame);

    graphicsScene = new QGraphicsScene();

    QPixmap background = QPixmap(":/images/images/default_background.png").scaled(350, 700);

    for (int column = 0; column < numberOfPlayers; column++) {
        QGraphicsView* currentGraphicsView = new QGraphicsView(graphicsScene, graphicsViewsGridFrame);
        currentGraphicsView->setStyleSheet("border: 3px solid white; border-radius: 0px;");
        currentGraphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        currentGraphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        graphicsViewsGrid->addWidget(currentGraphicsView, 0, column);

        currentGraphicsView->setBackgroundBrush(QBrush(background));

        graphicsViewsList.append(currentGraphicsView);
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

    /*
     *  create and connect short cut for accessing the chat window
     */
    QShortcut* openChatShortCut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_C), this);
    connect(openChatShortCut, &QShortcut::activated, this, &SchoolSkipperClient::handleVisibilityOfChat);

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
    if (chatWindow == nullptr) {
        qDebug() << "[ERR] No initialized chat window to display message";

        return;
    }

    isNewChatMessageAvailable = true;

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
        const int chatMenuHeight = (int) SchoolSkipper::CUSTOM_MENU_BAR_HEIGHT;

        if (isChatVisible) {
            const int x = width - (int) SchoolSkipper::CHAT_WINDOW_WIDTH;

            isNewChatMessageAvailable = false;

            if (isChatConnected) {
                chatConnectedStatus->setStyleSheet(chatConnectedStatus->styleSheet() + "color: green;");
                chatConnectedStatus->setText("Connected");
            } else {
                chatConnectedStatus->setStyleSheet(chatConnectedStatus->styleSheet() + "color: red;");
                chatConnectedStatus->setText("Disconnected");
            }

            chatWindow->resize((int) SchoolSkipper::CHAT_WINDOW_WIDTH, height - 70);
            chatWindow->move(QPoint(x, chatMenuHeight));

            chatConnectedStatus->resize(QSize(chatWindow->width() / 3 + 9, chatMenuHeight));
            chatConnectedStatus->move(QPoint(x, 0));

            reconnectButton->resize(QSize(chatWindow->width() / 3, chatMenuHeight));
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
            if (isNewChatMessageAvailable) {
                chatExpandButton->setIcon(QIcon(":/images/images/chat_message_new.png"));
            } else {
                chatExpandButton->setIcon(QIcon(":/images/images/chat.png"));
            }

            chatExpandButton->move(QPoint(width - chatExpandButton->width() + 3, chatMenuHeight + 30));
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
     *  Size calculation and style manipulation for graphics view grid and graphics scene backgrounds
     */
    if (isGraphicsViewsInitialized) {
        if (isChatVisible) {
            graphicsViewsGridFrame->resize(QSize(width - (int) SchoolSkipper::CHAT_WINDOW_WIDTH, height - (int) SchoolSkipper::CUSTOM_MENU_BAR_HEIGHT));
            graphicsViewsGridFrame->setStyleSheet(graphicsViewsGridFrame->styleSheet() + "border-right: 1px solid white; border-bottom-right-radius: 0px;");
        } else {
            graphicsViewsGridFrame->resize(QSize(width, height - (int) SchoolSkipper::CUSTOM_MENU_BAR_HEIGHT));
            graphicsViewsGridFrame->setStyleSheet(graphicsViewsGridFrame->styleSheet() + "border-right: 3px solid grey; border-bottom-right-radius: 10px;");
        }

        graphicsViewsGridFrame->show();
    }

    /*
     *  Size calculation and style manipulation for menu bar
     */
    if (isMenuBarInitialized) {
        if (isChatVisible) {
            menuBarFrame->resize(QSize(width - (int) SchoolSkipper::CHAT_WINDOW_WIDTH, (int) SchoolSkipper::CUSTOM_MENU_BAR_HEIGHT));
            menuBarFrame->setStyleSheet(menuBarFrame->styleSheet() + "border-top-right-radius: 0px; border-right: 1px solid white;");
        } else {
            menuBarFrame->resize(QSize(width, (int) SchoolSkipper::CUSTOM_MENU_BAR_HEIGHT));
            menuBarFrame->setStyleSheet(menuBarFrame->styleSheet() + "border-top-right-radius: 10px; border-right: 3px solid grey;");
        }

        menuBarFrame->show();
    }
}
