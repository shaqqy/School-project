#include "actor.h"

Actor::Actor(QObject *parent)
    : QObject{parent}
{
    setFlags(QGraphicsItem::ItemIsFocusable);
    setFlags(QGraphicsItem::ItemIsMovable);
    X = 155;
    Y = 275;
    setPixmap(QPixmap(":/images/images/blue-lik-right@2x.png"));
}

Actor::Actor(QPixmap *pix, bool isPlayer)
{
    if(isPlayer)
    {
        setFlags(QGraphicsItem::ItemIsFocusable);
        setFlags(QGraphicsItem::ItemIsMovable);
        X = 155;
        Y = 275;
        setPixmap(*pix);
        _isPlayer = true;

    }
    else
    {
        setFlags(QGraphicsItem::ItemIsMovable);
        X = 250;
        Y = 150;
        setPixmap(*pix);
        _isPlayer = false;
        setData(404, "npc");
    }
}

QRectF Actor::boundingRect() const
{
    qreal penWidth = 1;
    return QRectF(X-penWidth/2, Y-penWidth/2, pixmap().rect().width()/2+penWidth, pixmap().rect().height()/2+penWidth);
}

QPainterPath Actor::shape() const
{
    if(_isPlayer)
    {
        QPainterPath path;
        path.addRect(X+15,Y+49,20,1); //real size of the collider to be used
        return path;
    }
    else
    {
        //Check the images of the "monsters", give them IDs and set the collider according to the ID
        // TODO
        QPainterPath path;
        path.addRect(X, Y, 1, 1); // This is just a placeholder

        return path;
    }
}

void Actor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->drawPixmap(X,Y,50,50, pixmap()); //draw our pixmap with the size 50, 50 on the positon of X and Y
}

void Actor::fall()
{
    moveBy(0, gravity);
    Y += gravity;
}

void Actor::move(QPointF point)
{
    setPos(point);
}

void Actor::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()){
    case Qt::Key_Right:
        if(event->isAutoRepeat())
        {
            speedH = 20;
        }
        else
        {
            speedH = 20;
            setPixmap(QPixmap(":/images/images/blue-lik-right@2x.png"));
        }
        break;
    case Qt::Key_Left:
        if(event->isAutoRepeat())
        {
            speedH = -20;
        }
        else
        {
            speedH = -20;
            setPixmap(QPixmap());
        }
        break;
    case Qt::Key_Space:
        break;
    }
}

double Actor::getSpeedH() const
{
    return speedH;
}

void Actor::setSpeedH(double newSpeedH)
{
    speedH = newSpeedH;
}

double Actor::getSpeedV() const
{
    return speedV;
}

void Actor::setSpeedV(double newSpeedV)
{
    speedV = newSpeedV;
}

double Actor::getY() const
{
    return Y;
}

void Actor::setY(double newY)
{
    Y = newY;
}

double Actor::getX() const
{
    return X;
}

void Actor::setX(double newX)
{
    X = newX;
}