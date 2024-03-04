#ifndef GAME_H
#define GAME_H

#include <QObject>
#include "actor.h"
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPropertyAnimation>
#include "enums.h"
#include "platform.h"

class Game : public QObject
{
    Q_OBJECT
public:
    explicit Game(QObject *parent = nullptr);
    Actor* player;
    Actor* enemy;
    std::vector<Actor*> npcs;
    std::vector<Platform*> platforms;
    SchoolSkipper mode;

    double calculateDistance(QPointF item1, QPointF item2);

    QGraphicsScene *getScene() const;
    void setScene(QGraphicsScene *newScene);

    QGraphicsView *getView() const;
    void setView(QGraphicsView *newView);

    QGraphicsView *getEnemyView() const;
    void setEnemyView(QGraphicsView *newEnemyView);

public slots:
    void initPlatforms();
    void move();
    void moveNPCs();
    void moveEnemy(QPointF target);
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
