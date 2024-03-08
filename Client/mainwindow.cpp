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

    /*
     *  Initialization of application components. Chat window needs to be initialized first!
     */
    initMenuBar();
    initGraphicsViews();
}

SchoolSkipperClient::~SchoolSkipperClient() { delete ui; }

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

void SchoolSkipperClient::handleVisibilityOfChat() {
    if (chatFrame->isVisible()) {
        resize(QSize(this->width() - (int) SchoolSkipper::CHAT_WINDOW_WIDTH, this->height()));
    } else {
        resize(QSize(this->width() + (int) SchoolSkipper::CHAT_WINDOW_WIDTH, this->height()));
    }
}

void SchoolSkipperClient::scrollToEnd(QScrollBar *scrollBar) {
  scrollBar->setValue(scrollBar->maximum());

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

    /*
     *  Size calculation and style manipulation for menu bar
     */
    if (isMenuBarInitialized) {
        if (chatFrame->isVisible()) {
            menuBarFrame->resize(QSize(width - (int) SchoolSkipper::CHAT_WINDOW_WIDTH, (int) SchoolSkipper::CUSTOM_MENU_BAR_HEIGHT));
            menuBarFrame->setStyleSheet(menuBarFrame->styleSheet() + "border-top-right-radius: 0px; border-right: 1px solid white;");
        } else {
            menuBarFrame->resize(QSize(width, (int) SchoolSkipper::CUSTOM_MENU_BAR_HEIGHT));
            menuBarFrame->setStyleSheet(menuBarFrame->styleSheet() + "border-top-right-radius: 10px; border-right: 3px solid grey;");
        }

        menuBarFrame->show();
    }

    chatFrame->move(width - (int) SchoolSkipper::CHAT_WINDOW_WIDTH, 0);
    chatFrame->resize((int) SchoolSkipper::CHAT_WINDOW_WIDTH, height);
}
