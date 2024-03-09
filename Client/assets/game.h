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
#include <QRectF>
#include "chatframe.h"

class Game : public QObject
{
    Q_OBJECT
public:
    explicit Game(QObject *parent = nullptr);
    Actor* player;
    QPointF* LOP;
    Actor* opponent;
    QList<QPixmap*> pixmaps;
    QList<Actor*> npcs;
    QList<Platform*> platforms;
    SchoolSkipper mode;
    Network *network;
    ChatFrame *chat;

    double calculateDistance(QPointF item1, QPointF item2);

    QGraphicsScene *getScene() const;
    void setScene(QGraphicsScene *newScene);

    QGraphicsView *getView() const;
    void setView(QGraphicsView *newView);

    QGraphicsView *getEnemyView() const;
    void setEnemyView(QGraphicsView *newEnemyView);

    Network *getNetwork() const;
    void setNetwork(Network *newNetwork);


    void generateLevelSlice();
    void centerViewOnPlayArea();
    void initPlayer();
    ChatFrame *getChat() const;
    void setChat(ChatFrame *newChat);

public slots:
    void initEnemies();
    void initPlatforms();
    void move();
    void moveNPCs();
    void moveEnemy();
    void startSlot();

protected:
    QSize *viewportSize;
    QGraphicsScene *scene;
    QGraphicsView *view;
    QGraphicsView *enemyView;
    double score;
    double max;
    double gravity;
    QTimer *timer;
    void loadPixmaps();
signals:
    void startRepaint();
private:
    QGraphicsRectItem *invisibleArea;
    QPixmap* platformPix;
    QPixmap* npcPix;
    int ping;
    int difficulty;

};

#endif // GAME_H
