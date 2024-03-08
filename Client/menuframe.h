#ifndef MENUFRAME_H
#define MENUFRAME_H

#include <QFrame>
#include <QPushButton>
#include <QDebug>

#include "enums.h"


class MenuFrame : public QFrame
{
    Q_OBJECT

public:
    MenuFrame(QWidget* parent);

    const QIcon PLAY_ONLINE_ICON = QIcon(":/images/images/play_online.png");
    const QIcon PLAY_OFFLINE_ICON = QIcon("");

private:
    QPushButton* playOnlineButton;
    QPushButton* playOfflineButton;

    void initWidgets();
    void initWidgetsStyles();

protected:
    virtual void paintEvent(QPaintEvent* paint) override;
};

#endif // MENUFRAME_H
