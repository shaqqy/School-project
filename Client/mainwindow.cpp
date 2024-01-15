#include "mainwindow.h"
#include "ui_mainwindow.h"

SchoolSkipperClient::SchoolSkipperClient(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("School Skipper");
    setMinimumSize(QSize(329, 494));
    resize(QSize(900, 700));

    background = QPixmap(":/images/images/default_background_menu.png");

    initMainMenu();
    initChatWindow();

    client = new Network(this);
    //client->initUdpSocket(1234);
    //client->sendMessage(QByteArray("Connect"));
}

SchoolSkipperClient::~SchoolSkipperClient()
{
    delete ui;
}

bool SchoolSkipperClient::initMainMenu() {
    QPixmap playButtonPixmap = QPixmap(":/images/images/play_menu.png");

    playButton = new CustomButton(playButtonPixmap, this);
    playButton->show();

    return true;
}

void SchoolSkipperClient::initChatWindow() {
    chatWindow = new QTextBrowser(this);
    chatWindow->setStyleSheet("background-color: black; border: 3px solid grey; border-radius: 10px;");
    chatWindow->setTextColor(Qt::white);
    chatWindow->setText("[09:38] Test!!");
}

void SchoolSkipperClient::paintEvent(QPaintEvent*) {
    const int width = ui->centralwidget->width();
    const int height = ui->centralwidget->height();

    /*
     *  Position and size calculation for the game background
     */
    QPainter painter(this);
    QPixmap tmpBackground = QPixmap(background.scaled(width, height, Qt::KeepAspectRatio));

    gameBackgroundSize = tmpBackground.size();

    const int startBackground = (width / 2) - (tmpBackground.width() / 2);

    painter.drawPixmap(startBackground, 0, tmpBackground);

    /*
     *  Position and size calculation for the chat window
     */
    if ((width - (startBackground + tmpBackground.width())) > 60) {
        chatWindow->resize(width - (startBackground + tmpBackground.width() + 4), height - 50);
        chatWindow->move(QPoint(startBackground + tmpBackground.width() + 2, 0));
        chatWindow->show();
    } else {
        chatWindow->hide();
    }

    /*
     *  Position calculation for the play button.
     *  Size calculation is inside the buttons paintEvent function.
     */
    playButton->move(QPoint(startBackground + tmpBackground.width() / 3, tmpBackground.height() / 2));
}

QSize SchoolSkipperClient::getGameBackgroundSize() {
    return gameBackgroundSize;
}
