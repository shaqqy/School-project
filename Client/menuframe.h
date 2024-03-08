#ifndef MENUFRAME_H
#define MENUFRAME_H

#include <QFrame>
#include <QDebug>

class MenuFrame : public QFrame
{
    Q_OBJECT

public:
    MenuFrame(QWidget* parent);

protected:
    virtual void paintEvent(QPaintEvent* paint) override;
};

#endif // MENUFRAME_H
