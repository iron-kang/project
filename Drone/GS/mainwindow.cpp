#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "arduino-serial-lib.h"
#include "unistd.h"
#include <QTimer>
#include <stdio.h>
#include <stdlib.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    fd_sbus = serialport_init("/dev/ttyACM0", 115200);

    if (fd_sbus == -1)
        exit(0);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(getInfo()));
    timer->start(10);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getInfo()
{
    if (fd_sbus != -1)
    {
        read(fd_sbus, buffer, 18*sizeof(int));
        memcpy(sbus_channel, buffer, 18*sizeof(int));
        ui->ch1_val->setText(QString("%1").arg(sbus_channel[0]));
        ui->ch2_val->setText(QString("%1").arg(sbus_channel[1]));
        ui->ch3_val->setText(QString("%1").arg(sbus_channel[2]));
        ui->ch4_val->setText(QString("%1").arg(sbus_channel[3]));
        ui->ch5_val->setText(QString("%1").arg(sbus_channel[4]));
        ui->ch6_val->setText(QString("%1").arg(sbus_channel[5]));
        ui->ch7_val->setText(QString("%1").arg(sbus_channel[6]));
        ui->ch8_val->setText(QString("%1").arg(sbus_channel[7]));
        ui->ch9_val->setText(QString("%1").arg(sbus_channel[8]));
        ui->ch10_val->setText(QString("%1").arg(sbus_channel[9]));
        ui->ch11_val->setText(QString("%1").arg(sbus_channel[10]));
        ui->ch12_val->setText(QString("%1").arg(sbus_channel[11]));
        ui->ch13_val->setText(QString("%1").arg(sbus_channel[12]));
        ui->ch14_val->setText(QString("%1").arg(sbus_channel[13]));
        ui->ch15_val->setText(QString("%1").arg(sbus_channel[14]));
        ui->ch16_val->setText(QString("%1").arg(sbus_channel[15]));
        ui->ch17_val->setText(QString("%1").arg(sbus_channel[16]));
        ui->ch18_val->setText(QString("%1").arg(sbus_channel[17]));


    }
}
