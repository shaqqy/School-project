#ifndef GAME_H
#define GAME_H

#include <QObject>
#include "actor.h"
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPropertyAnimation>
#include <Network.h>
#include "enums.h"
#include "platform.h"
#include <QRandomGenerator>

class Game : public QObject
{
    Q_OBJECT
public:
    explicit Game(QObject *parent = nullptr);
    Actor* player;
    std::vector<QPointF*> L_O_P;
    std::vector<Actor*> opponents;
    std::vector<QPixmap*> pixmaps;
    std::vector<Actor*> npcs;
    std::vector<Platform*> platforms;
    SchoolSkipper mode;
    Network *network;
    double calculateDistance(QPointF item1, QPointF item2);

    QGraphicsScene *getScene() const;
    void setScene(QGraphicsScene *newScene);

    QGraphicsView *getView() const;
    void setView(QGraphicsView *newView);

    QGraphicsView *getEnemyView() const;
    void setEnemyView(QGraphicsView *newEnemyView);

    Network *getNetwork() const;
    void setNetwork(Network *newNetwork);


public slots:
    void initEnemies();
    void initPlatforms();
    void move();
    void moveNPCs();
    void moveEnemy();
    void startSlot(SchoolSkipper _mode);

protected:
    QGraphicsScene *scene;
    QGraphicsView *view;
    QGraphicsView *enemyView;
    double score;
    double max;
    double gravity;
    QTimer *timer;
signals:

};

#endif // GAME_H
