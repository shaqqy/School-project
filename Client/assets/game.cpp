#include "game.h"
#include <QDebug>
/*!
 * \brief Game::Game
 * \param parent
 * will create a new player and timer
 * will connect the timer timeout signal with the game move slot
 */
Game::Game(QObject *parent)
    : QObject{parent}
{
    player = new Actor(this);
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Game::move);

}
/*!
 * \brief Game::calculateDistance
 * \param item1
 * \param item2
 * \return
 */
double Game::calculateDistance(QPointF item1, QPointF item2)
{
    double result = std::sqrt(std::pow(item2.x()-item1.x(),2)+std::pow(item2.y()-item1.y(),2));
    return result;
}

/*!
 * \brief Game::move
 *
 * Handles movement of ingame entities
 */
void Game::move()
{
    //Start by calculating the score
    score = 150*std::abs(max);
    //Move the doodler by it's speed
    player->moveBy(player->getSpeedH(), player->getSpeedV());
    if(player->getY() < std::abs(max))
        max = player->getY();
    //Add gravity to the speed
    player->setSpeedV(player->getSpeedV()+gravity);
    //Check if the doodler went too low (loose condition)
    if(player->pos().ry() > 400)
    {
        //Player "fell down"
    }
    if(player->pos().ry() < 250){
        view->scroll(0,300);
    }
    //Check for collisions with npcs and platforms
    auto const collisions = scene->collidingItems(player);
    foreach (auto const &collision, collisions)
    {
        if(collision->data(404) == "platform"){
            if(player->getSpeedV() < 0)
            {
                player->setSpeedV(55);
            }
        }
        else if(collision->data(404) == "npc"){
            //Player dead
        }
    }
}
/*!
 * \brief Game::moveNPCs
 *
 * Will simply move enemies along one axis depending on the enemy
 * if it's no longer visible, reverse the direction of movement ->
 * TODO: figure out a way to delete the "older" npcs meaning a certain y distance
 * between them and the player
 */
void Game::moveNPCs()
{
    //Check if the enemy has speed Values set for it's horizontal and vertical speeds and move it accordingly
    foreach (auto const npc, npcs) {
        npc->moveBy(npc->getSpeedH(), npc->getSpeedV());
        if(!npc->isVisible())
        {
            npc->setSpeedH(npc->getSpeedH()*-1);
            npc->setSpeedV(npc->getSpeedV()*-1);
        }

    }
}
/*!
 * \brief Game::moveEnemy
 *
 * Connect this slot to the readyRead signal of the multicast socket
 * Whenever we receive a position update we'll add a property animation for the enemy player
 * The enemy player will send his position which we'll have to transpose by the size of our frame
 */
void Game::moveEnemy(QPointF target)
{
    QPropertyAnimation *animation = new QPropertyAnimation(enemy, "Geometry");
    auto enemyBoundingRect = enemy->boundingRect();
    animation->setStartValue(enemyBoundingRect);

    //Transpose the value
    animation->setEndValue(QRectF(target.x(), target.y(), enemyBoundingRect.width(), enemyBoundingRect.height()));
    animation->setDuration(25);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}
/*!
 * \brief Game::startSlot
 * \param _mode
 *
 * _mode defines whether we need to start in multiplayer mode or singleplayer
 * timer will be started with an interval of 25ms
 */
void Game::startSlot(SchoolSkipper _mode)
{
    mode = _mode;
    initPlatforms();
    timer->setInterval(25);
    timer->start();
}
/*!
 * \brief Game::getEnemyView
 * \return pointer to enemyView
 */
QGraphicsView *Game::getEnemyView() const
{
    return enemyView;
}
/*!
 * \brief Game::setEnemyView
 * \param newEnemyView
 */
void Game::setEnemyView(QGraphicsView *newEnemyView)
{
    enemyView = newEnemyView;
}

void Game::initPlatforms()
{
    QPixmap *plat = new QPixmap(":/images/C:/Users/mihi273307/Pictures/doodle_platform.png");
    for(int i = 0; i < 50; i++){
        Platform* tmp = new Platform(scene, plat);
        scene->addItem(tmp);
        tmp->mapToScene(300+i*120,300);
        tmp->show();
    }
}
/*!
 * \brief Game::getView
 * \return const value of view
 */
QGraphicsView *Game::getView() const
{
    return view;
}
/*!
 * \brief Game::setView
 * \param newView
 */
void Game::setView(QGraphicsView *newView)
{
    view = newView;
}
/*!
 * \brief Game::getScene
 * \return const value of scene
 */
QGraphicsScene *Game::getScene() const
{
    return scene;
}
/*!
 * \brief Game::setScene
 * \param newScene
 */
void Game::setScene(QGraphicsScene *newScene)
{
    scene = newScene;
}