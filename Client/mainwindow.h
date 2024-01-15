#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QLoggingCategory>
#include <QBitmap>

#include "network.h"
#include "assets/CustomButton.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class SchoolSkipperClient : public QMainWindow
{
    Q_OBJECT

public:
    SchoolSkipperClient(QWidget *parent = nullptr);
    ~SchoolSkipperClient();

    QSize getGameBackgroundSize();

private:
    Ui::MainWindow *ui;
    QPixmap background;

    Network* server;
    Network* client;

    QSize gameBackgroundSize;

    QTextBrowser* chatWindow;
    CustomButton* playButton;

    bool initMainMenu();
    void initChatWindow();

protected:
    virtual void paintEvent(QPaintEvent* paint) override;
};

#endif // MAINWINDOW_H
