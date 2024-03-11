#include "assets/game.h"
#include <QDebug>
/*!
 * \brief Game::Game
 * \param parent
 * will create a new player and timer
 * will connect the timer timeout signal with the game move slot
 */
Game::Game(QObject *parent) : QObject{parent} {
  player = new Actor(this);
  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &Game::move);
  connect(timer, &QTimer::timeout, this, &Game::moveNPCs);
  loadPixmaps();
  gravity = 1;
  ping = 0;
  difficulty = 1;
}

ChatFrame *Game::getChat() const { return chat; }

void Game::setChat(ChatFrame *newChat) { chat = newChat; }
void Game::loadPixmaps() {
  platformPix = new QPixmap(":/images/images/platform_standard.png");
  npcPix = new QPixmap(":/images/images/bug_1_2x.png");
}
/*!
 * \brief Game::getNetwork
 * convenience function
 * \return
 */
Network *Game::getNetwork() const { return network; }
/*!
 * \brief Game::setNetwork
 * convenience function
 * \param newNetwork
 */
void Game::setNetwork(Network *newNetwork) { network = newNetwork; }
/*!
 * \brief Game::calculateDistance
 * \param item1
 * \param item2
 * \return
 */
double Game::calculateDistance(QPointF item1, QPointF item2) {
  double result = std::sqrt(std::pow(item2.x() - item1.x(), 2) +
                            std::pow(item2.y() - item1.y(), 2));
  return result;
}

/*!
 * \brief Game::move
 *
 * Handles movement of ingame entities
 */
void Game::move() {
  ping++;
  // Start by calculating the score
  score = 150 * std::abs(max);
  // Check for collisions with npcs and platforms
  auto const collisions = scene->collidingItems(player);
  foreach (auto const &collision, collisions) {
    if (collision->data(404) == "platform") {
      if (player->getSpeedV() > 0) {
        player->setSpeedV(-20);
      }
    } else if (collision->data(404) == "npc") {
      if (player->getSpeedV() > 0)
        // Player dead
        disconnect(timer, &QTimer::timeout, this, &Game::move);
      if (mode == SchoolSkipper::Gamemode_Multiplayer)
        network->sendTcpMessage("Dead");
      // TODO: Show end score and game over
    }
  }
  // Add the decay of horizontal speed
  if (player->getSpeedH() > 0) {
    player->setSpeedH(player->getSpeedH() - 1);
  } else if (player->getSpeedH() < 0) {
    player->setSpeedH(player->getSpeedH() + 1);
  }
  // Move the doodler by it's speed
  player->moveBy(player->getSpeedH(), player->getSpeedV());
  if (player->getY() < std::abs(max))
    max = player->getY();
  // Add gravity to the speed
  player->setSpeedV(player->getSpeedV() + gravity);
  // Check if the doodler went too low (loose condition)
  QPointF viewport_pos = getView()->mapFromScene(player->pos());
  // qDebug() << "Viewpos" << viewport_pos;
  qDebug() << "Scenepos" << player->pos();
  if (viewport_pos.y() > 560) {
    if (this->mode == SchoolSkipper::Gamemode_Singleplayer) {
      timer->stop();
      // Player fell down
      // TODO: Show end score and game over
    } else {
      // Player "fell down"
      disconnect(timer, &QTimer::timeout, this, &Game::move);
      network->sendTcpMessage("Dead");
      // TODO: Show end score and game over
    }
  }
  // Scroll higher if the player reached over a certain point in the screen
  if (viewport_pos.y() < 80 /*&& ping % 5 == 0*/) {
    //    view->scroll(0, -300);
    ping = 0;
    view->centerOn(QPointF(viewportSize->width() / 2, player->pos().y()));
    //    view->setTransformationAnchor(QGraphicsView::NoAnchor);
    //    view->translate(viewportSize->width()/2,player->pos().y()-300);
    //    view->verticalScrollBar()->setValue(view->verticalScrollBar()->value()-500);
    //    view->QAbstractScrollArea::scroll(0,player->pos().y()-400);
    generateLevelSlice();
  }
  if (mode == SchoolSkipper::Gamemode_Multiplayer) {
    int x, y;
    x = player->pos().x();
    y = player->pos().y();
    QString msg = QString::number(x) + "x" + QString::number(y);
    network->sendUdpMessage(QByteArray(msg.toUtf8()));
  }
  //    qDebug() << player->pos();
  //    emit startRepaint();
}

/*!
 * \brief Game::moveNPCs
 *
 * Will simply move enemies along one axis depending on the enemy
 * if it's no longer visible, reverse the direction of movement ->
 * TODO: figure out a way to delete the "older" npcs meaning a certain y
 * distance between them and the player
 */
void Game::moveNPCs() {
  // Check if the enemy has speed Values set for it's horizontal and vertical
  // speeds and move it accordingly
  Q_FOREACH (auto const npc, npcs) {
    npc->moveBy(npc->getSpeedH(), npc->getSpeedV());
    if (!npc->isVisible()) {
      npc->setSpeedH(npc->getSpeedH() * -1);
      npc->setSpeedV(npc->getSpeedV() * -1);
    }
  }
}
/*!
 * \brief Game::moveEnemy
 *
 * Connect this slot to the readyRead signal of the multicast socket
 * Whenever we receive a position update we'll add a property animation for the
 * enemy player The enemy player will send his position which we'll have to
 * transpose by the size of our frame
 */
void Game::moveEnemy() {
  if (*LOP != opponent->pos()) {
    if (LOP->x() < opponent->pos().x()) {
      opponent->setPixmap(*opponent->pixmaps.back());
    } else {
      opponent->setPixmap(*opponent->pixmaps.front());
    }
    QPropertyAnimation *animation =
        new QPropertyAnimation(opponent, "Geometry");
    auto enemyBoundingRect = opponent->boundingRect();
    animation->setStartValue(enemyBoundingRect);

    // Transpose the value
    animation->setEndValue(QRectF(LOP->x(), LOP->y(), enemyBoundingRect.width(),
                                  enemyBoundingRect.height()));
    animation->setDuration(25);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
  }
}
/*!
 * \brief Game::startSlot
 * \param _mode
 *
 * _mode defines whether we need to start in multiplayer mode or singleplayer
 * timer will be started with an interval of 25ms
 */
void Game::startSlot() {
  QObject *sender = QObject::sender();
  viewportSize = new QSize(view->size());
  initPlayer();
  initPlatforms();
  if (sender->objectName() == "Singleplayer") {
    mode = SchoolSkipper::Gamemode_Singleplayer;
    timer->setInterval(25);
    timer->start();
  } else {
    mode = SchoolSkipper::Gamemode_Multiplayer;
    opponent = new Actor();
    opponent->setOpacity(0.4);
    LOP = new QPointF();
    connect(timer, &QTimer::timeout, this, &Game::moveEnemy);
    network->sendTcpMessage(QByteArray::fromStdString("Ready"));
  }

  //  invisibleArea = new QGraphicsRectItem(QRectF(0, -(view->size().height() /
  //  2),
  //                                               view->size().width(),
  //                                               view->size().height() / 2));
  //  invisibleArea->setBrush(QBrush(QColor::fromRgb(0, 0, 0, 0)));
  //  scene->addItem(invisibleArea);
}

void Game::startFromServer(){
    timer->setInterval(25);
    timer->start();
}

void Game::initPlayer() {
  scene->addItem(player);
  player->setPos(300, -viewportSize->height() / 2);
  player->setZValue(100);
  player->show();
  player->setSpeedV(0);
}
/*!
 * \brief Game::getEnemyView
 * \return pointer to enemyView
 */
QGraphicsView *Game::getEnemyView() const { return enemyView; }
/*!
 * \brief Game::initEnemies
 */
void Game::initEnemies() {
  int rnd = QRandomGenerator::global()->bounded(1, 3);
  for (int i = 0; i < rnd; i++) {
    Actor *enemy = new Actor(pixmaps.front(), false);
    npcs.push_back(enemy);
  }
}

void Game::centerViewOnPlayArea() {
  view->centerOn(
      QPointF(view->size().width() / 2, -(view->size().height() / 2)));
}

/*!
 * \brief Game::setEnemyView
 * \param newEnemyView
 */
void Game::setEnemyView(QGraphicsView *newEnemyView) {
  enemyView = newEnemyView;
}
/*!
 * \brief Game::initPlatforms sets up a "test level" with platforms
 */
void Game::initPlatforms() {
  Platform *doodleStart = new Platform(scene, platformPix);
  scene->addItem(doodleStart);
  doodleStart->setPos(QPointF(view->size().width() / 2 - 50,
                              -viewportSize->height() / 2 + 150));
  doodleStart->show();
  for (int i = 0; i < 20; i++) {
    Platform *tmp = new Platform(scene, platformPix);
    scene->addItem(tmp);
    //        tmp->mapToScene(300+i*10,300);
    // Put them from middle to left
    if (i % 3 == 0) {
      tmp->setPos(
          QPointF(i * QRandomGenerator::global()->bounded(1, 12), (-i * 20)));
    }
    // Put them from middle to right
    else if (i % 2 == 0) {
      tmp->setPos(QPointF(i * QRandomGenerator::global()->bounded(1, 9) + 300,
                          -i * 15));
    } else {
      tmp->setPos(QPointF(i + 150, -i * 15));
    }
    tmp->show();
    platforms.push_back(tmp);
  }
   for (int i = 0; i < 15; i++) {
     Platform *tmp = new Platform(this, platformPix);
     scene->addItem(tmp);
     tmp->setPos(QPointF(i * platformPix->width(), 0));
     tmp->show();
     platforms.push_back(tmp);
   }
}
void Game::generateLevelSlice() {
  if (difficulty < 20)
    difficulty++;
  // Get Player Position as Point
  QPointF playerpos = player->pos();
  // Generate platforms as a multiple from a random distance the player can jump
  // (about 60px so 45-65) (the harder the difficulty the closer to the max
  // maybe?) Place them somewhere on X within the boundaries of the viewport
  QPointF lastPlaced;
  QRandomGenerator rnd;
  for (int i = 0; i < 10; i++) {
    Platform *tmp = new Platform(scene, platformPix);
    if (i == 0) {
      lastPlaced =
          QPointF(rnd.bounded(0, viewportSize->rwidth()),
                  playerpos.y() - rnd.bounded(5 + difficulty, 25 + difficulty));
      tmp->setPos(lastPlaced);
    } else {
      lastPlaced = QPointF(rnd.bounded(0, viewportSize->rwidth()),
                           lastPlaced.y() -
                               rnd.bounded(5 + difficulty, 25 + difficulty));
    }
  }
  // Generate platforms and npcs above the view
  for (int i = 0; i < (20 - difficulty % 10); i++) {
    Platform *tmp = new Platform(scene, platformPix);
    scene->addItem(tmp);
    //        tmp->mapToScene(300+i*10,300);
    // Put them from middle to left
    if (i % 3 == 0) {
      lastPlaced = QPointF(
          QRandomGenerator::global()->bounded(0, viewportSize->rwidth()),
          playerpos.y() -
              QRandomGenerator::global()->bounded(45 + difficulty, 65));
      tmp->setPos(lastPlaced);
      //      tmp->setPos(
      //          QPointF(i * QRandomGenerator::global()->bounded(1, 12), (-i *
      //          20)));
    }
    // Put them from middle to right
    else if (i % 2 == 0) {
      tmp->mapToParent(
          (QRandomGenerator::global()->bounded((viewportSize->width() / 3) * 2,
                                               viewportSize->width())),
          player->pos().y() - i * 10);
      //      tmp->setPos(QPointF(i * QRandomGenerator::global()->bounded(1, 9)
      //      + 300,
      //                          -i * 15));
    } else {
      tmp->mapToParent(
          (QRandomGenerator::global()->bounded(
              (viewportSize->width() / 3), (viewportSize->width() / 3) * 2)),
          player->pos().y() - i * 10);
      //      tmp->setPos(QPointF(i + 150, -i * 15));
    }
    tmp->show();
    platforms.push_back(tmp);
  }
}
/*!
 * \brief Game::getView
 * \return const value of view
 */
QGraphicsView *Game::getView() const { return view; }
/*!
 * \brief Game::setView
 * \param newView
 */
void Game::setView(QGraphicsView *newView) { view = newView; }
/*!
 * \brief Game::getScene
 * \return const value of scene
 */
QGraphicsScene *Game::getScene() const { return scene; }
/*!
 * \brief Game::setScene
 * \param newScene
 */
void Game::setScene(QGraphicsScene *newScene) { scene = newScene; }
