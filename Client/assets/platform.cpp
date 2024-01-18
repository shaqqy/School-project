#include "platform.h"

Platform::Platform(QObject *parent)
    : QObject{parent}
{

}

Platform::Platform(QObject *parent, QPixmap *pix)
{
    this->setParent(parent);
    setPixmap(*pix);
}

QRectF Platform::boundingRect() const
{
    return pixmap().rect();
}

QPainterPath Platform::shape() const
{
    QPainterPath path;
    path.addRect(pixmap().rect());
    return path;
}

void Platform::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->drawPixmap(this->x(), this->y(), pixmap());
}
