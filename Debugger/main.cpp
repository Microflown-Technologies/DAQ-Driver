#include "mainwindow.h"
#include "DAQDriver.h"
#include "SerialConnector/GenericSerialConnector.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    GenericSerialConnector serial;
    serial.voyagerConnected();
    w.show();
    return a.exec();
}
