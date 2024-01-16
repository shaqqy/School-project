#include "mainwindow.h"
#include "ui_mainwindow.h"

SchoolSkipperClient::SchoolSkipperClient(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("School Skipper");
    setMinimumSize(QSize(329, 500));
    resize(QSize(900, 300));
    initGameFrames();
    initChatWindow();

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
    opponentFrame->setStyleSheet("border-top: 3px solid grey; border-bottom: 3px solid grey; border-left: 3px solid grey; border-top-left-radius: 10px; border-bottom-left-radius: 10px;");
    opponentFrame->openMainMenu();
    opponentFrame->show();
}

void SchoolSkipperClient::initChatWindow() {
    chatWindow = new QTextBrowser(this);
    chatWindow->setStyleSheet("background-color: black; border-top: 3px solid grey; border-right: 3px solid grey; border-top-right-radius: 10px;");
    chatWindow->setTextColor(Qt::white);
}

void SchoolSkipperClient::paintEvent(QPaintEvent*) {
    const int width = ui->centralwidget->width();
    const int height = ui->centralwidget->height();

    const int gameFramesEndCoordinates = mainFrame->width() + opponentFrame->width();

    /*
     *  Position calculation for main and opponent frame
     */
    opponentFrame->move(QPoint(3, 0));
    mainFrame->move(QPoint(opponentFrame->width() + 3, 0));
    /*
     *  Position and size calculation for the chat window
     */
    if ((width - (gameFramesEndCoordinates)) > 60) {
        chatWindow->resize(width - (gameFramesEndCoordinates + 4), height - 50);
        chatWindow->move(QPoint(gameFramesEndCoordinates, 0));
        chatWindow->show();
    } else {
        chatWindow->hide();
    }
}
