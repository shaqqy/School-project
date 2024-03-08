#include "mainwindow.h"
#include "ui_mainwindow.h"

SchoolSkipperClient::SchoolSkipperClient(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    isMenuBarInitialized = false;
    isGraphicsViewsInitialized = false;

    numberOfPlayers = 2;

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
    initGraphicsViews();
    /*
   * Do this here after the network init!
   */
    game = new Game(this);
    game->setNetwork(networker);
    /*
   * Brain too tired - I just assume the first entry is the one for singleplayer
   */
    game->setScene(this->graphicsScene);
    game->setView(this->graphicsViewsList.front());
    game->startSlot(SchoolSkipper::Gamemode_Singleplayer);
    this->grabKeyboard();
}

SchoolSkipperClient::~SchoolSkipperClient()
{
    delete ui;
}

void SchoolSkipperClient::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Left:
        if (event->isAutoRepeat()) {
            if (game->player->getSpeedH() > -20) {
                game->player->setSpeedH(game->player->getSpeedH() - 3);
            }
        } else {
            //      game->player->setSpeedH(game->player->getSpeedH() * -1);
            //      game->player->setPixmap(*game->player->pixmaps.back());
            if (game->player->getSpeedH() > -20) {
                if (game->player->getSpeedH() > 0) {
                    game->player->setSpeedH(-5);
                    game->player->setPixmap(*game->player->pixmaps.back());
                    break;
                }
                game->player->setSpeedH(game->player->getSpeedH() - 2);
            } else if (game->player->getSpeedH() < -20) {
                game->player->setSpeedH(-20);
            }
            game->player->setPixmap(*game->player->pixmaps.back());
        }
        break;
    case Qt::Key_Right:
        if (event->isAutoRepeat()) {
            if (game->player->getSpeedH() < 20) {
                if (game->player->getSpeedH() < 0) {
                    game->player->setSpeedH(5);
                    game->player->setPixmap(*game->player->pixmaps.front());
                    break;
                }
                game->player->setSpeedH(game->player->getSpeedH() + 2);
            }
        } else {
            if (game->player->getSpeedH() < 20) {
                if (game->player->getSpeedH() < 0)
                    game->player->setSpeedH(5);
                game->player->setSpeedH(game->player->getSpeedH() + 2);
            }
            game->player->setPixmap(*game->player->pixmaps.front());
        }
        break;
    case Qt::Key_Return:
//        messageReadyToSendSlot();
//        chatBox->clear();
        break;
    case Qt::Key_Backspace:
//        chatBox->setText(QString::fromStdString(
//            chatBox->text().toStdString().substr(0, chatBox->text().length() - 1)));
        break;
//    default:
//        chatBox->setText(chatBox->text() + event->text());
        /* Here could go the handling for shooting, if we want to implement that */
    }
}

void SchoolSkipperClient::keyReleaseEvent(QKeyEvent *event)
{

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
}
