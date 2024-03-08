#include "mainwindow.h"
#include "ui_mainwindow.h"

SchoolSkipperClient::SchoolSkipperClient(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    isGraphicsViewsInitialized = false;

    numberOfPlayers = 2;

    ui->setupUi(this);

    initWindowDefaultParams();

    expandChatFrameButton = new QPushButton(this);
    expandChatFrameButton->setStyleSheet("background: white; border-left: 3px solid grey; border-top: 3px solid grey; border-bottom: 3px solid grey; border-top-left-radius: 20px; border-bottom-left-radius: 20px;");
    expandChatFrameButton->setIcon(CHAT_EXPAND_ICON_DEFAULT);
    expandChatFrameButton->show();

    initFrames();
    initNetwork();

    /*
     *  connect the networker with the chat frame for message exchange and connnection status exchange
     */
    connect(networker, &Network::newChatMessage, chatFrame, &ChatFrame::newMessage);
    connect(chatFrame, &ChatFrame::sendMessageOverNetwork, networker, &Network::sendTcpMessage);
    connect(networker, &Network::tcpConnectionStatus, chatFrame, &ChatFrame::handleConnectionChange);

    /*
     *  connect minimize and maximize buttons for handling the visibility of the chat frame
     */
    connect(this, &SchoolSkipperClient::visibilityChangeOfChatFrame, this, &SchoolSkipperClient::handleVisibilityChangeOfChatFrame);
    connect(expandChatFrameButton, &QPushButton::clicked, this, &SchoolSkipperClient::expandChatFrame);
    connect(chatFrame, &ChatFrame::minimizeChatFrame, this, &SchoolSkipperClient::minimizeChatFrame);

    initGraphicsViews();
}

SchoolSkipperClient::~SchoolSkipperClient()
{
    delete ui;
}

void SchoolSkipperClient::initWindowDefaultParams() {
    setWindowTitle(WINDOW_TITLE);
    setWindowIcon(WINDOW_ICON);
    setStyleSheet("background-color: white;");
    resize(QSize(900, 700));
}

void SchoolSkipperClient::initFrames() {
    chatFrame = new ChatFrame(this);
    chatFrame->hide();

    menuFrame = new MenuFrame(this);
    menuFrame->move(0, 0);
    menuFrame->show();
}

void SchoolSkipperClient::initNetwork() {
    networker = new Network(this);
    networker->initTcpSocket();
}

void SchoolSkipperClient::handleVisibilityChangeOfChatFrame(bool visible) {
    if (visible) {
        resize(width() + (int) SchoolSkipper::CHAT_WINDOW_WIDTH, height());

        menuFrame->setStyleSheet(menuFrame->styleSheet() + "border-right: 1px solid white; border-top-right-radius: 0px;");
        expandChatFrameButton->hide();
        chatFrame->show();

        return;
    }

    resize(width() - (int) SchoolSkipper::CHAT_WINDOW_WIDTH, height());

    menuFrame->setStyleSheet(menuFrame->styleSheet() + "border-right: 3px solid grey; border-top-right-radius: 10px;");
    expandChatFrameButton->show();
    chatFrame->hide();
}

void SchoolSkipperClient::expandChatFrame() {
    emit visibilityChangeOfChatFrame(true);
}

void SchoolSkipperClient::minimizeChatFrame() {
    emit visibilityChangeOfChatFrame(false);
}

void SchoolSkipperClient::initGraphicsViews() {
    qDebug() << "[SYS] Initializing graphics views ...";

    graphicsViewsGridFrame = new QFrame(this);
    graphicsViewsGridFrame->move(QPoint(0, (int) SchoolSkipper::CUSTOM_MENU_BAR_HEIGHT));
    graphicsViewsGridFrame->stackUnder(expandChatFrameButton);
    graphicsViewsGridFrame->setStyleSheet("background-color: black; border-bottom-left-radius: 10px; border: 3px solid grey; border-top: 1px solid white;");

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

void SchoolSkipperClient::paintEvent(QPaintEvent*) {
    const int width = ui->centralwidget->width();
    const int height = ui->centralwidget->height();

    /*
     *  Size calculation and style manipulation for graphics view grid and graphics scene backgrounds
     */
    if (isGraphicsViewsInitialized) {
        if (chatFrame->isVisible()) {
            graphicsViewsGridFrame->resize(QSize(width - (int) SchoolSkipper::CHAT_WINDOW_WIDTH, height - (int) SchoolSkipper::CUSTOM_MENU_BAR_HEIGHT));
            graphicsViewsGridFrame->setStyleSheet(graphicsViewsGridFrame->styleSheet() + "border-right: 1px solid white; border-bottom-right-radius: 0px;");
        } else {
            graphicsViewsGridFrame->resize(QSize(width, height - (int) SchoolSkipper::CUSTOM_MENU_BAR_HEIGHT));
            graphicsViewsGridFrame->setStyleSheet(graphicsViewsGridFrame->styleSheet() + "border-right: 3px solid grey; border-bottom-right-radius: 10px;");
        }

        graphicsViewsGridFrame->show();
    }

    if (chatFrame->isVisible()) {
        chatFrame->move(width - (int) SchoolSkipper::CHAT_WINDOW_WIDTH, 0);
        chatFrame->resize((int) SchoolSkipper::CHAT_WINDOW_WIDTH, height);

        menuFrame->resize(width - (int) SchoolSkipper::CHAT_WINDOW_WIDTH, (int) SchoolSkipper::CUSTOM_MENU_BAR_HEIGHT);
    } else {
        expandChatFrameButton->resize(40, 40);
        expandChatFrameButton->move(width - expandChatFrameButton->width(), (int) SchoolSkipper::CHAT_EXPAND_BUTTON_Y_POS);

        menuFrame->resize(width, (int) SchoolSkipper::CUSTOM_MENU_BAR_HEIGHT);
    }
}
