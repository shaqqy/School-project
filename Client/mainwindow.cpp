#include "mainwindow.h"
#include "ui_mainwindow.h"

SchoolSkipperClient::SchoolSkipperClient(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("School Skipper");
    setBackground(":/images/images/default_background_menu.png");
    initMainMenu();

    client = new Network(this);
    client->initSocket(1234);
    client->sendMessage(QByteArray("Connect"));
}

SchoolSkipperClient::~SchoolSkipperClient()
{
    delete ui;
}

bool SchoolSkipperClient::setBackground(QString path) {
    try {
        QPixmap background = QPixmap(path);
        background = background.scaled(size(), Qt::KeepAspectRatio);

        QPalette palette;
        palette.setBrush(QPalette::Window, background);
        this->setPalette(palette);
    } catch (...) {
        qDebug() << "[SYS] Failed to set background";
        return false;
    }

    return true;
}

bool SchoolSkipperClient::initMainMenu() {
    QPixmap playButtonPixmap = QPixmap(":/images/images/play_menu.png");
    QIcon playButtonIcon = QIcon(playButtonPixmap);

    ui->playButton->resize(playButtonPixmap.size());

    ui->playButton->setMask(playButtonPixmap.mask());

    ui->playButton->setIcon(playButtonIcon);
    ui->playButton->setIconSize(playButtonPixmap.size());

    return true;
}
