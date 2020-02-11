#ifdef QT_IS_AVAILABLE
#include "QtSerialConnector.h"

QtSerialConnector::QtSerialConnector()
{
    connect(this, &QSerialPort::errorOccurred, this, &QtSerialConnector::on_errorOccurred);
    open();
}

std::vector<uint8_t> QtSerialConnector::read() {
    if(bytesAvailable()) {
        QByteArray readData = readAll();
        return std::vector<uint8_t>(readData.begin(), readData.end());
    } else {
        return std::vector<uint8_t>();
    }
}

void QtSerialConnector::write(const std::vector<uint8_t> &data) {
    if(!QSerialPort::isOpen()) return;
    QSerialPort::write(reinterpret_cast<const char*>(data.data()), data.size());
}

void QtSerialConnector::priorityWrite(const std::vector<uint8_t> &data) {
    write(data);
    flush();
}

size_t QtSerialConnector::dataAvailable() const {
    return QSerialPort::bytesAvailable();
}

bool QtSerialConnector::isOpen() {
    return QSerialPort::isOpen();
}

void QtSerialConnector::close()
{
    qInfo() << "Succesfully closed DAQ serial port";

    QSerialPort::close();
}

bool QtSerialConnector::voyagerConnected() {
    return true;
}

QSerialPortInfo QtSerialConnector::getSerialPortInfo() const {
    for(auto port: QSerialPortInfo::availablePorts()) {
        if(port.serialNumber() == "01234567")
            return port;
        if (port.manufacturer() == "Microflown Technologies")
            return port;
    }
    QSerialPortInfo serialPortInfo("ttyGS0");
    return serialPortInfo;
}

bool QtSerialConnector::open() {
    if(!QSerialPort::isOpen()) {
        setPort(getSerialPortInfo());
        setBaudRate(4000000);
        if(!QSerialPort::open(QIODevice::ReadWrite)) {
            qWarning() << "Failed to open DAQ serial port" << errorString();
            return false;
        } else {
            clear();
            qInfo() << "Succesfully openend DAQ serial port";
            return true;
        }
    } else {
        qDebug() << "Cannot open DAQ serial port because it is already openend";
        return false;
    }
}

void QtSerialConnector::on_errorOccurred(QSerialPort::SerialPortError error) {
    if (error != QSerialPort::NoError) qWarning() << "DAQ Serial Port error:" <<QSerialPort::errorString();
}
#endif
