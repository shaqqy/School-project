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
  QPixmap *enemy_test = new QPixmap(":/images/images/bug_1_2x.png");
  pixmaps.push_back(enemy_test);
  connect(timer, &QTimer::timeout, this, &Game::move);
  connect(timer, &QTimer::timeout, this, &Game::moveNPCs);
  gravity = 1;
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
        if(mode == SchoolSkipper::Gamemode_Multiplayer)
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
//  qDebug() << "Viewpos" << viewport_pos;
//  qDebug() << "Scenepos" << player->pos();
  if (viewport_pos.y() > 560) {
    if (this->mode == SchoolSkipper::Gamemode_Singleplayer) {
      // Player fell down
      // TODO: Show end score and game over
    } else {
      // Player "fell down"
      network->sendTcpMessage("Dead");
      // TODO: Show end score and game over
    }
  }
  // Scroll higher if the player reached over a certain point in the screen
  if (viewport_pos.y() < 250) {
    view->scroll(0, 300);
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
  foreach (auto const npc, npcs) {
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
  for (int i = 0; i < opponents.size(); i++) {
    QPropertyAnimation *animation =
        new QPropertyAnimation(opponents.at(i), "Geometry");
    auto enemyBoundingRect = opponents.at(i)->boundingRect();
    animation->setStartValue(enemyBoundingRect);

    // Transpose the value
    animation->setEndValue(QRectF(LOP.at(i)->x(), LOP.at(i)->y(),
                                  enemyBoundingRect.width(),
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
void Game::startSlot(SchoolSkipper _mode) {
  mode = _mode;
  player->setPos(150, 0);
  player->setZValue(100);
  scene->addItem(player);
  viewportSize = new QSize(view->size());
  player->show();
  player->setFocus(Qt::FocusReason::OtherFocusReason);
  player->setSpeedV(-20);
  if (mode == SchoolSkipper::Gamemode_Multiplayer) {
    for (int i = 0; i < network->LOP.size(); i++) {
      LOP.push_back(network->LOP.at(i));
    }
    connect(timer, &QTimer::timeout, this, &Game::moveEnemy);
  }
  initPlatforms();
  timer->setInterval(20);
  timer->start();
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
  QPixmap *plat = new QPixmap(":/images/images/platform_standard.png");
  for (int i = 0; i < 20; i++) {
    Platform *tmp = new Platform(scene, plat);
    scene->addItem(tmp);
    //        tmp->mapToScene(300+i*10,300);
    // Put them from middle to left
    if (i % 2 == 0) {
      tmp->setPos(
          QPointF(i * QRandomGenerator::global()->bounded(1, 12), i * 20));
    }
    // Put them from middle to right
    else {
      tmp->setPos(
          QPointF(i * QRandomGenerator::global()->bounded(1, 9) + 300, i * 15));
    }
    tmp->show();
    platforms.push_back(tmp);
  }
}
void Game::generateLevelSlice(){
    //Generate platforms and npcs above the view
    //
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
