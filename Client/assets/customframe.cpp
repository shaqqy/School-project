#include "customframe.h"

CustomFrame::CustomFrame(QPixmap background, QWidget* parent) : QFrame(parent)
{
    this->parent = parent;
    this->background = background;

    multiplayerButton = nullptr;
    playButton = nullptr;
}

void CustomFrame::paintEvent(QPaintEvent*) {
    QPainter painter = QPainter(this);

    QPixmap tmpBackground = background.scaled(parent->width(), parent->height(), Qt::KeepAspectRatio);

    resize(QSize(tmpBackground.width(), parent->height()));

    /*
     *  Position menu buttons when there initialized
     */
    if (playButton != nullptr && multiplayerButton != nullptr) {
        playButton->move(QPoint((width() / 2) + width() / 6, height() / 2));
        multiplayerButton->move(QPoint(width() / 6, height() / 2));
    }

    painter.drawPixmap(3, 3, tmpBackground.width() - 3, tmpBackground.height() - 5, tmpBackground);
}

void CustomFrame::openMainMenu() {
    multiplayerButton = new CustomButton(QPixmap(":/images/images/multiplayer.png"), this);
    multiplayerButton->show();

    playButton = new CustomButton(QPixmap(":/images/images/play_menu.png"), this);
    playButton->show();
}
