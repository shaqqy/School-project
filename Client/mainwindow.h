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

#include "network.h"
#include "assets/customframe.h"

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
    void chatConnectionStatusSlot(bool connected);

signals:
    QByteArray messageReadyToSendSignal(QByteArray message);

private:
    Ui::MainWindow *ui;

    CustomFrame* mainFrame;
    CustomFrame* opponentFrame;

    QGraphicsView* graphicsViewMainFrame;
    QGraphicsScene* graphicsScene;
    QGraphicsPixmapItem* graphicsSceneBackground;

    QGraphicsView* graphicsViewOpponentFrame;
    QGraphicsScene* graphicsSceneOpponentFrame;
    QGraphicsPixmapItem* graphicsSceneBackgroundOpponentFrame;

    Network* server;
    Network* networker;

    QPushButton* chatButton;
    QTextBrowser* chatWindow;
    QLineEdit* chatBox;

    QPushButton* sendButton;
    QPushButton* reconnectButton;
    QLabel* chatConnectionStatus;

    bool isMenuActive;
    bool isChatVisible;

    void initGameFrames();
    void initChatWindow();
    void initGraphicsViewAndScene();

    void handleVisibilityOfChat();

protected:
    virtual void paintEvent(QPaintEvent* paint) override;
};

#endif // MAINWINDOW_H
