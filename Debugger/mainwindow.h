#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//Qt Framework
#include <QTimer>
#include <QMainWindow>

//Internal headers
#include "SystemAudioDriver.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_audioTimerTimeout();

private:
    Ui::MainWindow *ui;
    QTimer m_audioTimer;
    SystemAudioDriver m_audioDriver;
};
#endif // MAINWINDOW_H
