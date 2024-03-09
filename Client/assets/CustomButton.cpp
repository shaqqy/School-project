#include "CustomButton.h"

CustomButton::CustomButton(QPixmap pixmap, QWidget* parent) : QPushButton(nullptr, parent) {
    this->pixmap = pixmap;
    this->parent = parent;

    //connect();
}

void CustomButton::onClick() {

}

void CustomButton::paintEvent(QPaintEvent*) {
    QPainter painter = QPainter(this);

    int width = (parent->size().width() / 4);
    int height = (parent->size().height() / 5);

    if (width > 200) { width = 200; }
    if (height > 100) { height = 100; }

    QPixmap tmpButtonPixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio);

    resize(QSize(tmpButtonPixmap.width(), tmpButtonPixmap.height()));

    painter.drawPixmap(0, 0, tmpButtonPixmap);
}
