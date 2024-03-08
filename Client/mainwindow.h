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

private:
    Ui::MainWindow *ui;

    Network* networker;
    ChatFrame* chatFrame;
    MenuFrame* menuFrame;

    QPushButton* expandChatFrame;

    QFrame* graphicsViewsGridFrame;
    QGridLayout* graphicsViewsGrid;
    QGraphicsScene* graphicsScene;
    QList<QGraphicsPixmapItem*> graphicsSceneBackgroundsList;
    QList<QGraphicsView*> graphicsViewsList;

    bool isGraphicsViewsInitialized;
    bool isMenuBarInitialized;

    int numberOfPlayers;

    void initGraphicsViews();

protected:
    virtual void paintEvent(QPaintEvent* paint) override;
};

#endif // MAINWINDOW_H
