#ifndef ACTOR_H
#define ACTOR_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPainterPath>
#include <QPainter>
#include <QTimer>
#include <QKeyEvent>
class Actor : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Actor(QObject *parent = nullptr);
    Actor(QPixmap *pix, bool isPlayer = false);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void fall();

    double getX() const;
    void setX(double newX);

    double getY() const;
    void setY(double newY);

    double getSpeedV() const;
    void setSpeedV(double newSpeedV);

    double getSpeedH() const;
    void setSpeedH(double newSpeedH);
    std::vector<QPixmap*> pixmaps;

public slots:
    void move(QPointF point);


signals:

protected:
    void keyReleaseEvent(QKeyEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    double X;
    double Y;
    double speedV;
    double speedH;
    bool _isPlayer;
    double gravity = 3;
};
#endif // ACTOR_H
