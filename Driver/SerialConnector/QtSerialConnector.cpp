#include "QtSerialConnector.h"

QtSerialConnector::QtSerialConnector()
{
    openConnection();
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
    QSerialPort::write(reinterpret_cast<const char*>(data.data()), data.size());
}

void QtSerialConnector::priorityWrite(const std::vector<uint8_t> &data) {
    flush();
    write(data);
    flush();
}

uint64_t QtSerialConnector::dataAvailable() const {
    return QSerialPort::bytesAvailable();
}

bool QtSerialConnector::voyagerConnected() {
    for(auto port: QSerialPortInfo::availablePorts()) {
        if (port.manufacturer() == "Microflown Technologies")
            return true;
    }
    return false;
}

QSerialPortInfo QtSerialConnector::getSerialPortInfo() const {
    for(auto port: QSerialPortInfo::availablePorts()) {
        if (port.manufacturer() == "Microflown Technologies")
            return port;
    }
    QSerialPortInfo serialPortInfo("ttyGS0");
    return serialPortInfo;
}

void QtSerialConnector::openConnection() {
    if(!isOpen()) {
        qDebug() << QSerialPortInfo::standardBaudRates();
        setPort(getSerialPortInfo());
        setBaudRate(4000000);
        if(!open(QIODevice::ReadWrite)) {
            qWarning() << "Failed to open DAQ serial port" << errorString();
        } else {
            clear();
            qInfo() << "Succesfully openend DAQ serial port";
        }
    } else {
        qDebug() << "Cannot open DAQ serial port because it is already openend";
    }
}
