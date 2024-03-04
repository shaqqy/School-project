#include "customframe.h"

CustomFrame::CustomFrame(QPixmap background, QWidget* parent) : QFrame(parent)
{
    this->parent = parent;
    this->background = background;
    setBaseSize(background.size());
    multiplayerButton = nullptr;
    playButton = nullptr;
}

CustomButton *CustomFrame::getMultiplayerButton() const
{
    return multiplayerButton;
}

void CustomFrame::setMultiplayerButton(CustomButton *newMultiplayerButton)
{
    multiplayerButton = newMultiplayerButton;
}

CustomButton *CustomFrame::getPlayButton() const
{
    return playButton;
}

void CustomFrame::setPlayButton(CustomButton *newPlayButton)
{
    playButton = newPlayButton;
}

void CustomFrame::paintEvent(QPaintEvent*) {
    QPainter painter = QPainter(this);
    QPixmap tmpBackground = background.scaled(parent->width(), parent->height(), Qt::KeepAspectRatio);

    resize(QSize(tmpBackground.width(), parent->height()));

    /*
     *  Position menu buttons when there initialized
     */
    if (playButton != nullptr && multiplayerButton != nullptr) {
        playButton->move(QPoint((width() / 2) + (multiplayerButton->width() / 2), height() / 2));
        multiplayerButton->move(QPoint(multiplayerButton->width() / 2, height() / 2));
    }

    painter.drawPixmap(3, 3, tmpBackground.width() - 3, tmpBackground.height() - 6, tmpBackground);
}

void CustomFrame::openMainMenu() {
    multiplayerButton = new CustomButton(QPixmap(":/images/images/multiplayer.png"), this);
    multiplayerButton->show();

    playButton = new CustomButton(QPixmap(":/images/images/play_menu.png"), this);
    playButton->show();
}
