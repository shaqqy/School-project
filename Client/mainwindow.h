#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QLoggingCategory>
#include <QBitmap>

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

private:
    Ui::MainWindow *ui;

    CustomFrame* mainFrame;
    CustomFrame* opponentFrame;

    Network* server;
    Network* client;

    QTextBrowser* chatWindow;
    CustomButton* playButton;

    void initGameFrames();
    void initChatWindow();

protected:
    virtual void paintEvent(QPaintEvent* paint) override;
};

#endif // MAINWINDOW_H
