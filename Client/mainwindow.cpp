#include "mainwindow.h"
#include "ui_mainwindow.h"

SchoolSkipperClient::SchoolSkipperClient(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    isGraphicsViewsInitialized = false;

    numberOfPlayers = 1;

    ui->setupUi(this);

    initWindowDefaultParams();

    expandChatFrameButton = new QPushButton(this);
    expandChatFrameButton->setStyleSheet(
        "background: white; border-left: 3px solid grey; border-top: 3px solid "
        "grey; border-bottom: 3px solid grey; border-top-left-radius: 20px; "
        "border-bottom-left-radius: 20px;");
    expandChatFrameButton->setIcon(CHAT_EXPAND_ICON_DEFAULT);
    expandChatFrameButton->show();

    initFrames();
    initNetwork();

    /*
   *  connect the networker with the chat frame for message exchange and
   * connnection status exchange
   */
    connect(networker, &Network::newChatMessage, chatFrame,
            &ChatFrame::newMessage);
    connect(chatFrame, &ChatFrame::sendMessageOverNetwork, networker,
            &Network::sendTcpMessage);
    connect(networker, &Network::tcpConnectionStatus, chatFrame,
            &ChatFrame::handleConnectionChange);

    /*
   *  connect minimize and maximize buttons for handling the visibility of the
   * chat frame. And connect reconnect button of chat frame and exchange of
   * expand button icon.
   */
    connect(this, &SchoolSkipperClient::visibilityChangeOfChatFrame, this,
            &SchoolSkipperClient::handleVisibilityChangeOfChatFrame);
    connect(expandChatFrameButton, &QPushButton::clicked, this,
            &SchoolSkipperClient::expandChatFrame);
    connect(chatFrame, &ChatFrame::minimizeChatFrame, this,
            &SchoolSkipperClient::minimizeChatFrame);
    connect(chatFrame, &ChatFrame::reconnectChat, this,
            &SchoolSkipperClient::reconnectTcp);
    connect(chatFrame, &ChatFrame::changeIconOfExpandButton, this,
            &SchoolSkipperClient::handleIconChangeOfExpandChatButton);

    initGraphicsViews();
    networker->initTcpSocket();

    /*
   * Do this here after the network init!
   */
    game = new Game(this);
    game->setNetwork(networker);
    connect(networker, &Network::startFromServer, game, &Game::startFromServer);
    game->setScene(this->graphicsScene);
    game->setView(this->graphicsViewsList.front());
    game->centerViewOnPlayArea();
    //  QGraphicsPixmapItem *singleplayer = new
    //  QGraphicsPixmapItem(QPixmap(":/images/images/play_menu.png"));
    //  this->graphicsScene->addItem(singleplayer);
    //  singleplayer->setPos(250,-350);
    //  connect(singleplayer, &QGraphicsItem::)
    initStartButtons();
    //  QPushButton *singleplayer = new
    //  QPushButton(QIcon(QPixmap(":/images/images/play_menu.png")));
    //  game->startSlot(SchoolSkipper::Gamemode_Singleplayer);
    grabKeyboard();
}

void SchoolSkipperClient::initStartButtons() {
    QPixmap *singleplayerPix = new QPixmap(":/images/images/play_menu.png");
    single = new CustomButton(*singleplayerPix, this);
    graphicsScene->addWidget(single);
    single->setGeometry(270, -350, singleplayerPix->width(),
                        singleplayerPix->height());
    single->setObjectName("Singleplayer");
    single->setFixedSize(singleplayerPix->size());
    connect(single, &QPushButton::clicked, game, &Game::startSlot);
    connect(single, &QPushButton::clicked, this,
            &SchoolSkipperClient::hideStartButtons);

    QPixmap *multiplayerPix = new QPixmap(":/images/images/multiplayer.png");
    multi = new CustomButton(*multiplayerPix, this);
    graphicsScene->addWidget(multi);
    multi->setGeometry(270, -150, multiplayerPix->width(),
                       multiplayerPix->height());
    multi->setObjectName("Multiplayer");
    multi->setFixedSize(multiplayerPix->size());
    connect(multi, &QPushButton::clicked, game, &Game::startSlot);
    connect(multi, &QPushButton::clicked, this,
            &SchoolSkipperClient::hideStartButtons);
}

void SchoolSkipperClient::hideStartButtons() {
    single->hide();
    multi->hide();
}

SchoolSkipperClient::~SchoolSkipperClient() { delete ui; }

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

void SchoolSkipperClient::initNetwork() { networker = new Network(this); }

void SchoolSkipperClient::handleVisibilityChangeOfChatFrame(bool visible) {
    if (visible) {
        resize(width() + (int)SchoolSkipper::CHAT_WINDOW_WIDTH, height());

        menuFrame->setStyleSheet(
            menuFrame->styleSheet() +
            "border-right: 1px solid white; border-top-right-radius: 0px;");
        expandChatFrameButton->hide();
        chatFrame->show();

        return;
    }

    resize(width() - (int)SchoolSkipper::CHAT_WINDOW_WIDTH, height());

    menuFrame->setStyleSheet(
        menuFrame->styleSheet() +
        "border-right: 3px solid grey; border-top-right-radius: 10px;");
    expandChatFrameButton->show();
    chatFrame->hide();
}

void SchoolSkipperClient::handleIconChangeOfExpandChatButton(QIcon icon) {
    expandChatFrameButton->setIcon(icon);
}

void SchoolSkipperClient::expandChatFrame() {
    emit visibilityChangeOfChatFrame(true);
}

void SchoolSkipperClient::minimizeChatFrame() {
    emit visibilityChangeOfChatFrame(false);
}

void SchoolSkipperClient::reconnectTcp() { networker->initTcpSocket(); }

void SchoolSkipperClient::initGraphicsViews() {
    qDebug() << "[SYS] Initializing graphics views ...";

    graphicsViewsGridFrame = new QFrame(this);
    graphicsViewsGridFrame->move(
        QPoint(0, (int)SchoolSkipper::CUSTOM_MENU_BAR_HEIGHT));
    graphicsViewsGridFrame->stackUnder(expandChatFrameButton);
    graphicsViewsGridFrame->setStyleSheet(
        "background-color: black; border-bottom-left-radius: 10px; border: 3px "
        "solid grey; border-top: 1px solid white;");

    graphicsViewsGrid = new QGridLayout(graphicsViewsGridFrame);

    graphicsScene = new QGraphicsScene();

    QPixmap background =
        QPixmap(":/images/images/default_background.png").scaled(350, 700);

    for (int column = 0; column < numberOfPlayers; column++) {
        QGraphicsView *currentGraphicsView =
            new QGraphicsView(graphicsScene, graphicsViewsGridFrame);
        currentGraphicsView->setStyleSheet(
            "border: 3px solid white; border-radius: 0px;");
        currentGraphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        currentGraphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        graphicsViewsGrid->addWidget(currentGraphicsView, 0, column);

        currentGraphicsView->setBackgroundBrush(QBrush(background));

        graphicsViewsList.append(currentGraphicsView);
    }

    isGraphicsViewsInitialized = true;

    qDebug() << "[SYS] Initialized graphics views";
}

void SchoolSkipperClient::paintEvent(QPaintEvent *) {
    const int width = ui->centralwidget->width();
    const int height = ui->centralwidget->height();

    /*
   *  Size calculation and style manipulation for graphics view grid and
   * graphics scene backgrounds
   */
    if (isGraphicsViewsInitialized) {
        if (chatFrame->isVisible()) {
            graphicsViewsGridFrame->resize(
                QSize(width - (int)SchoolSkipper::CHAT_WINDOW_WIDTH,
                      height - (int)SchoolSkipper::CUSTOM_MENU_BAR_HEIGHT));
            graphicsViewsGridFrame->setStyleSheet(
                graphicsViewsGridFrame->styleSheet() +
                "border-right: 1px solid white; border-bottom-right-radius: 0px;");
        } else {
            graphicsViewsGridFrame->resize(
                QSize(width, height - (int)SchoolSkipper::CUSTOM_MENU_BAR_HEIGHT));
            graphicsViewsGridFrame->setStyleSheet(
                graphicsViewsGridFrame->styleSheet() +
                "border-right: 3px solid grey; border-bottom-right-radius: 10px;");
        }

        graphicsViewsGridFrame->show();
    }

    if (chatFrame->isVisible()) {
        chatFrame->move(width - (int)SchoolSkipper::CHAT_WINDOW_WIDTH, 0);
        chatFrame->resize((int)SchoolSkipper::CHAT_WINDOW_WIDTH, height);

        menuFrame->resize(width - (int)SchoolSkipper::CHAT_WINDOW_WIDTH,
                          (int)SchoolSkipper::CUSTOM_MENU_BAR_HEIGHT);
    } else {
        expandChatFrameButton->resize(40, 40);
        expandChatFrameButton->move(width - expandChatFrameButton->width(),
                                    (int)SchoolSkipper::CHAT_EXPAND_BUTTON_Y_POS);

        menuFrame->resize(width, (int)SchoolSkipper::CUSTOM_MENU_BAR_HEIGHT);
    }
}

/*
 * Controller Function
 */

void SchoolSkipperClient::keyPressEvent(QKeyEvent *event) {
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
        chatFrame->sendMessagePressed();
        chatFrame->getInputBox()->clear();
        //        messageReadyToSendSlot();
        //        chatBox->clear();
        break;
    case Qt::Key_Backspace:
        chatFrame->getInputBox()->setText(QString::fromStdString(
            chatFrame->getInputBox()->text().toStdString().substr(
                0, chatFrame->getInputBox()->text().length() - 1)));
        break;
    default:
        chatFrame->getInputBox()->setText(chatFrame->getInputBox()->text() +
                                          event->text());
        //    default:
        //        chatBox->setText(chatBox->text() + event->text());
        /* Here could go the handling for shooting, if we want to implement that */
    }
}
