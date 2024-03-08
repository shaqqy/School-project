#include "menuframe.h"

MenuFrame::MenuFrame(QWidget* parent) : QFrame(parent) {
    qDebug() << "[SYS] Initializing menu frame ...";

    setStyleSheet("background-color: black; border-left: 3px solid grey; border-top: 3px solid grey; border-top-left-radius: 10px;");

}

void MenuFrame::paintEvent(QPaintEvent* paint) {

}
