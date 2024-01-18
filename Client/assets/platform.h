#ifndef PLATFORM_H
#define PLATFORM_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QPainter>
class Platform : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Platform(QObject *parent = nullptr);
    Platform(QObject* parent = nullptr, QPixmap* pix = nullptr);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

signals:

};

#endif // PLATFORM_H
