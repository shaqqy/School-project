#include "app.h"
#include "ui_app.h"

App::App(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::App)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(this);
    player = new Actor(scene);
    QPixmap *pix = new QPixmap("qrc:/C:/Users/mihi273307/Downloads/Doodle Jump/Doodle Jump/bottom-bug-tiles.png");

    player->setPixmap(pix->copy());
    scene->addItem(player);
    view->setScene(scene);

}

App::~App()
{
    delete ui;
}

