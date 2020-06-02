#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&m_audioTimer, &QTimer::timeout, this, &MainWindow::on_audioTimerTimeout);
    connect(ui->controlWidget, &Controls::connected, this, &MainWindow::on_connected);
    m_audioTimer.start(10);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    QMainWindow::closeEvent(event);
}

void MainWindow::on_audioTimerTimeout()
{

}

void MainWindow::on_connected()
{
//    ui->controlWidget->daqDriver()->streaming()->addNewBufferCallback(std::bind(&MainWindow::on_newBuffer, this));

}

void MainWindow::on_newBuffer()
{
    while(ui->controlWidget->daqDriver()->streaming()->pendingBufferCount()) {
        ui->widget->processBuffer(ui->controlWidget->daqDriver()->streaming()->nextQueuedBuffer());
    }
}



