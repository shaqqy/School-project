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

#include "enums.h"
#include "network.h"


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

    Network* server;
    Network* networker;

    QFrame* graphicsViewsGridFrame;
    QGridLayout* graphicsViewsGrid;
    QGraphicsScene* graphicsScene;
    QList<QGraphicsPixmapItem*> graphicsSceneBackgroundsList;
    QList<QGraphicsView*> graphicsViewsList;

    QFrame* menuBarFrame;

    QPushButton* chatExpandButton;
    QPushButton* chatMinimizeButton;
    QTextBrowser* chatWindow;
    QLineEdit* chatBox;

    QPushButton* sendButton;
    QPushButton* reconnectButton;
    QLabel* chatConnectedStatus;

    bool isChatVisible;
    bool isChatConnected;
    bool isNewChatMessageAvailable;

    bool isGraphicsViewsInitialized;
    bool isChatWindowInitialized;
    bool isMenuBarInitialized;

    int numberOfPlayers;

    void initMenuBar();
    void initChatWindow();
    void initGraphicsViews();

    void handleVisibilityOfChat();
    void scrollToEnd(QScrollBar* scrollBar);

protected:
    virtual void paintEvent(QPaintEvent* paint) override;
};

#endif // MAINWINDOW_H
