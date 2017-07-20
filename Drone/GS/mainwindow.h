#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    int fd_sbus;
    char buffer[100];
    int sbus_channel[18];

private slots:
    void getInfo();
};

#endif // MAINWINDOW_H
