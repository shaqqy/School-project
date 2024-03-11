#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QLoggingCategory>
#include <QBitmap>
#include <QStyle>
#include <QGraphicsView>
#include <QRect>
#include <QGraphicsPixmapItem>
#include <QLineEdit>
#include <QTime>
#include <QLabel>
#include <QScrollBar>
#include <QPushButton>
#include <QList>
#include <QGridLayout>
#include <QShortcut>
#include "assets/game.h"
#include "enums.h"
#include "network.h"
#include "chatframe.h"
#include "menuframe.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class SchoolSkipperClient : public QMainWindow
{
    Q_OBJECT

public:
    SchoolSkipperClient(QWidget *parent = nullptr);
    ~SchoolSkipperClient();

    const QString WINDOW_TITLE = "School Skipper";
    const QIcon WINDOW_ICON = QIcon(":/images/images/icon.png");

    const QIcon CHAT_EXPAND_ICON_DEFAULT = QIcon(":/images/images/chat.png");
    const QIcon CHAT_EXPAND_ICON_NEW_MESSAGE = QIcon(":/images/images/chat_new_message.png");
    Game *game;
    void initStartButtons();
signals:
    void visibilityChangeOfChatFrame(bool visibile);

public slots:
    void handleVisibilityChangeOfChatFrame(bool visible);
    void expandChatFrame();
    void minimizeChatFrame();
    void reconnectTcp();

    void keyPressEvent(QKeyEvent *event) override;
    void hideStartButtons();
private:
    Ui::MainWindow *ui;

    Network* networker;
    ChatFrame* chatFrame;
    MenuFrame* menuFrame;

    QPushButton* expandChatFrameButton;
    QPushButton* single;
    QPushButton* multi;
    QFrame* graphicsViewsGridFrame;
    QGridLayout* graphicsViewsGrid;
    QGraphicsScene* graphicsScene;
    QList<QGraphicsPixmapItem*> graphicsSceneBackgroundsList;
    QList<QGraphicsView*> graphicsViewsList;

    bool isGraphicsViewsInitialized;
    bool isMenuBarInitialized;

    int numberOfPlayers;

    void initGraphicsViews();

    void initWindowDefaultParams();
    void initFrames();
    void initNetwork();

protected:
    virtual void paintEvent(QPaintEvent* paint) override;
};

#endif // MAINWINDOW_H
