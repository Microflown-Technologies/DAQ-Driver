#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&m_audioTimer, &QTimer::timeout, this, &MainWindow::on_audioTimerTimeout);
    m_audioTimer.start(10);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    ui->widget->daqDriver().disconnect();
    QMainWindow::closeEvent(event);
}

void MainWindow::on_audioTimerTimeout()
{

}



