#include "menuframe.h"

MenuFrame::MenuFrame(QWidget* parent) : QFrame(parent) {
    qDebug() << "[SYS] Initializing menu frame ...";

    setStyleSheet("background-color: black; border-left: 3px solid grey; border-top: 3px solid grey; border-right: 3px solid grey; border-top-right-radius: 10px; border-top-left-radius: 10px;");

    initWidgets();

    qDebug() << "[SYS] Initialized menu frame";
}

void MenuFrame::initWidgets() {
    playOnlineButton = new QPushButton(this);
    playOnlineButton->show();

    playOfflineButton = new QPushButton(this);
    playOfflineButton->show();

    initWidgetsStyles();
}

void MenuFrame::initWidgetsStyles() {
    playOnlineButton->setIcon(PLAY_ONLINE_ICON);
    playOnlineButton->setStyleSheet("border: 2px solid white; border-bottom-left-radius: 10px; border-bottom-right-radius: 10px; border-top-left-radius: 10px; border-top-right-radius: 10px;");

    playOfflineButton->setIcon(PLAY_OFFLINE_ICON);
    playOfflineButton->setStyleSheet("border: 2px solid white; border-bottom-left-radius: 10px; border-bottom-right-radius: 10px; border-top-left-radius: 10px; border-top-right-radius: 10px;");
}

void MenuFrame::paintEvent(QPaintEvent*) {
    const int marginTopAndBottom = 8;
    const int marginLeft = 10;
    const int margin = marginTopAndBottom / 2;

    playOnlineButton->resize((int) SchoolSkipper::CUSTOM_MENU_BAR_ITEMS_WIDTH, height() - marginTopAndBottom);
    playOnlineButton->move(width() - playOnlineButton->width() - marginLeft, margin);

    playOfflineButton->resize((int) SchoolSkipper::CUSTOM_MENU_BAR_ITEMS_WIDTH, height() - marginTopAndBottom);
    playOfflineButton->move(width() - playOfflineButton->width() - playOnlineButton->width() - marginLeft, margin);
}
