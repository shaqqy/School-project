#ifndef APP_H
#define APP_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "actor.h"
#include <ui_app.h>
QT_BEGIN_NAMESPACE
namespace Ui { class QMainWindow; }
QT_END_NAMESPACE

class App : public QMainWindow
{
    Q_OBJECT

public:
    App(QWidget *parent = nullptr);
    ~App();

protected:
    /* This area has to be deleted after testing */
    QGraphicsScene *scene;
    QGraphicsView *view;
    /* ---------------Area end------------------ */
    Actor* player;
    Actor* enemyPlayer;
    std::vector<Actor*> enemies;

private:
    Ui::App *ui;
};
#endif // APP_H
