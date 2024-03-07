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
#include "assets/game.h"
#include "enums.h"
#include "network.h"
#include <QtMath>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class SchoolSkipperClient : public QMainWindow
{
    Q_OBJECT

public:
    SchoolSkipperClient(QWidget *parent = nullptr);
    ~SchoolSkipperClient();

public slots:
    void messageReadyToSendSlot();
    void chatMessageReadySlot(QString message);
    void chatConnectedStatusSlot(bool connected);

signals:
    QByteArray messageReadyToSendSignal(QByteArray message);

private:
    Ui::MainWindow *ui;

    QFrame* graphicsViewsGridFrame;
    QGridLayout* graphicsViewsGrid;
    QGraphicsScene* graphicsScene;
    QGraphicsPixmapItem* graphicsSceneBackground;
    QList<QGraphicsView*> graphicsViewsList;

    Network* server;
    Network* networker;
    Game* game;
    QPushButton* chatExpandButton;
    QPushButton* chatMinimizeButton;
    QTextBrowser* chatWindow;
    QLineEdit* chatBox;

    QPushButton* sendButton;
    QPushButton* reconnectButton;
    QLabel* chatConnectedStatus;

    bool isChatWindowInitialized;
    bool isChatVisible;
    bool isChatConnected;

    bool isGraphicsViewsInitialized;

    int numberOfPlayers;

    void initChatWindow();
    void initGraphicsViews();

    void handleVisibilityOfChat();
    void scrollToEnd(QScrollBar* scrollBar);

protected:
    virtual void paintEvent(QPaintEvent* paint) override;
};

#endif // MAINWINDOW_H
