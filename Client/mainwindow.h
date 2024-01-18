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
    void messageReadySlot();

signals:
    QByteArray messageReadySignal(QByteArray message);

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
    Network* client;

    QTextBrowser* chatWindow;
    QPushButton* sendButton;
    QLineEdit* chatBox;

    bool isMenuActive;

    void initGameFrames();
    void initChatWindow();
    void initGraphicsViewAndScene();

protected:
    virtual void paintEvent(QPaintEvent* paint) override;
};

#endif // MAINWINDOW_H
