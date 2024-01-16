#ifndef ACTOR_H
#define ACTOR_H

#include <QObject>
#include <QGraphicsPixmapItem>

class Actor : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Actor(QObject *parent = nullptr);

public slots:
    void move(QPointF point);

protected:
    bool isPlayer;

signals:

};

#endif // ACTOR_H
