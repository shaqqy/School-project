#include "platform.h"

Platform::Platform(QObject *parent)
    : QObject{parent}
{

}

Platform::Platform(QObject *parent, QPixmap *pix)
{
    this->setParent(parent);
    setPixmap(*pix);
    setData(404,"platform");
}

QRectF Platform::boundingRect() const
{
    return QRectF(pos().x(),pos().y(),pixmap().rect().width(),pixmap().height());
}

QPainterPath Platform::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void Platform::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->drawPixmap(this->x(), this->y(), pixmap());
    painter->setPen(QColor::fromRgb(255,0,0,255));
    painter->drawRect(boundingRect());
}
