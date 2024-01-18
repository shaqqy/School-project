#include "mainwindow.h"
#include "ui_mainwindow.h"

SchoolSkipperClient::SchoolSkipperClient(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("School Skipper");
    setWindowIcon(QIcon(":/images/images/icon.png"));
    resize(QSize(900, 600));

    sendButton = nullptr;
    chatBox = nullptr;

    initGameFrames();
    initChatWindow();
    initGraphicsViewAndScene();

    client = new Network(this);
    //client->initUdpSocket(1234);
    //client->sendMessage(QByteArray("Connect"));
}

SchoolSkipperClient::~SchoolSkipperClient()
{
    delete ui;
}

void SchoolSkipperClient::initGameFrames() {
    mainFrame = new CustomFrame(QPixmap(":/images/images/default_background_menu.png"), this);
    mainFrame->setStyleSheet("border-top: 3px solid grey; border-bottom: 3px solid grey; border-left: 3px solid grey;");
    mainFrame->show();

    opponentFrame = new CustomFrame(QPixmap(":/images/images/default_background.png"), this);
    opponentFrame->setStyleSheet("border-top: 3px solid grey; border-bottom: 3px solid grey; border-left: 3px solid grey;");
    opponentFrame->openMainMenu();
    isMenuActive = true;
    opponentFrame->show();
}

void SchoolSkipperClient::initChatWindow() {
    chatWindow = new QTextBrowser(this);
    chatWindow->setStyleSheet("background-color: black; border-top: 3px solid grey; border-right: 3px solid grey; border-top-right-radius: 10px; border-left: 3px solid grey");
    chatWindow->setTextColor(Qt::white);

    sendButton = new QPushButton(this);
    sendButton->setStyleSheet("border-right: 3px solid grey; border-bottom: 3px solid grey; border-top: 1px solid white; border-bottom-right-radius: 10px; background-color: black;");
    sendButton->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
    sendButton->show();

    chatBox = new QLineEdit(this);
    chatBox->setPlaceholderText("Type Message ...");
    chatBox->setTextMargins(QMargins(5, 0, 0, 0));
    chatBox->setStyleSheet("background-color: dark-grey; color: white; border: none; border-top: 1px solid white; border-bottom: 3px solid grey; border-left: 3px solid grey;");
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

void SchoolSkipperClient::paintEvent(QPaintEvent*) {
    const int width = ui->centralwidget->width();
    const int height = ui->centralwidget->height();

    const int gameFramesEndCoordinates = mainFrame->width() + opponentFrame->width();

    setMinimumWidth(gameFramesEndCoordinates + 5);
    setMaximumWidth(gameFramesEndCoordinates + 60);

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
    if ((width - (gameFramesEndCoordinates)) > 60) {
        chatWindow->resize(width - (gameFramesEndCoordinates + 4), height - 50);
        chatWindow->move(QPoint(gameFramesEndCoordinates, 0));

        if (sendButton != nullptr) {
            sendButton->resize(QSize(chatWindow->width() / 4, 50));
            sendButton->move(QPoint(gameFramesEndCoordinates + (chatWindow->width() - chatWindow->width() / 4), chatWindow->height()));
        }

        if (chatBox != nullptr) {
            chatBox->resize(QSize(chatWindow->width() - (chatWindow->width() / 4), 50));
            chatBox->move(QPoint(gameFramesEndCoordinates, chatWindow->height()));
        }

        if (chatWindow->isHidden() && sendButton->isHidden() && chatBox->isHidden()) {
            chatWindow->show();
            sendButton->show();
            chatBox->show();
        }
    } else if (!chatWindow->isHidden() && !sendButton->isHidden() && !chatBox->isHidden()) {
        mainFrame->setStyleSheet("border-top: 3px solid grey; border-bottom: 3px solid grey; border-left: 3px solid grey; border-right: 3px solid grey;");

        chatWindow->hide();
        sendButton->hide();
        chatBox->hide();
    }
}
