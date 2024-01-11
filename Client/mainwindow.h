#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QLoggingCategory>
#include <QBitmap>

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

private:
    Ui::MainWindow *ui;
    Network* server;
    Network* client;

    bool setBackground(QString path);
    bool initMainMenu();
};

#endif // MAINWINDOW_H
