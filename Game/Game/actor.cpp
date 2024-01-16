#include "actor.h"

Actor::Actor(QObject *parent)
    : QObject{parent}
{

}

void Actor::move(QPointF point)
{
    setPos(point);
}
