#ifndef GAME_H
#define GAME_H

#include <QObject>
#include "actor.h"
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPropertyAnimation>
class Game : public QObject
{
    Q_OBJECT
public:
    explicit Game(QObject *parent = nullptr);
    Actor* player;
    Actor* enemy;
    std::vector<Actor*> npcs;
    void startGame();
    void scoreGame();

    QGraphicsScene *getScene() const;
    void setScene(QGraphicsScene *newScene);

    QGraphicsView *getView() const;
    void setView(QGraphicsView *newView);

public slots:
    void move();
    void moveNPCs();
    void moveEnemy(QPointF target);

protected:
    QGraphicsScene *scene;
    QGraphicsView *view;
    double score;
    double max;
    double gravity;
    QTimer *timer;
signals:

};

#endif // GAME_H
