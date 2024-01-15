#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H

#include <QPushButton>
#include <QPainter>
#include <QDebug>
#include <QBitmap>
#include <QPaintEvent>

class CustomButton : public QPushButton {
    Q_OBJECT

public:
    CustomButton(QPixmap pixmap, QWidget* parent);

private:
    QPixmap pixmap;
    QWidget* parent;

protected:
    virtual void paintEvent(QPaintEvent* paint) override;

};

#endif // CUSTOMBUTTON_H
