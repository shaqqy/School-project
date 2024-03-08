#include "mainwindow.h"
#include "ui_mainwindow.h"

SchoolSkipperClient::SchoolSkipperClient(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    isMenuBarInitialized = false;
    isGraphicsViewsInitialized = false;

    numberOfPlayers = 3;

    ui->setupUi(this);

    setWindowTitle("School Skipper");
    setWindowIcon(QIcon(":/images/images/icon.png"));
    setStyleSheet("background-color: white;");
    resize(QSize(350 * numberOfPlayers, 700));

    networker = new Network(this);
    networker->initTcpSocket();

    chatFrame = new ChatFrame(this);
    chatFrame->show();

    expandChatFrame = new QPushButton(this);
    expandChatFrame->show();

    menuFrame = new MenuFrame(this);
    menuFrame->show();

    /*
     * connect the networker with the chat frame
     */
    connect(networker, &Network::newChatMessage, chatFrame, &ChatFrame::newMessage);
    connect(chatFrame, &ChatFrame::sendMessageOverNetwork, networker, &Network::sendTcpMessage);

    initGraphicsViews();
}

SchoolSkipperClient::~SchoolSkipperClient()
{
    delete ui;
}

void SchoolSkipperClient::initGraphicsViews() {
    qDebug() << "[SYS] Initializing graphics views ...";

    graphicsViewsGridFrame = new QFrame(this);
    graphicsViewsGridFrame->move(QPoint(0, (int) SchoolSkipper::CUSTOM_MENU_BAR_HEIGHT));
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

    chatFrame->move(width - (int) SchoolSkipper::CHAT_WINDOW_WIDTH, 0);
    chatFrame->resize((int) SchoolSkipper::CHAT_WINDOW_WIDTH, height);

    expandChatFrame->resize(width / 30, width / 30);
    expandChatFrame->move(width - expandChatFrame->width(), (int) SchoolSkipper::CHAT_EXPAND_BUTTON_Y_POS);

    menuFrame->move(0, 0);

    if (chatFrame-isVisible()) {
        menuFrame->resize(width - (int) SchoolSkipper::CHAT_WINDOW_WIDTH, (int) SchoolSkipper::CUSTOM_MENU_BAR_HEIGHT);
    } else {
        menuFrame->resize(width, (int) SchoolSkipper::CUSTOM_MENU_BAR_HEIGHT);
    }
}
