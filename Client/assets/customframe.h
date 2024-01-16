#ifndef CUSTOMFRAME_H
#define CUSTOMFRAME_H

#include <QFrame>
#include <QPainter>

#include "CustomButton.h"

class CustomFrame : public QFrame
{
    Q_OBJECT

public:
    CustomFrame(QPixmap background, QWidget* parent);

private:
    QWidget* parent;
    QPixmap background;

    CustomButton* multiplayerButton;
    CustomButton* playButton;

public slots:
    void openMainMenu();

protected:
    virtual void paintEvent(QPaintEvent* paint) override;
};

#endif // CUSTOMFRAME_H
