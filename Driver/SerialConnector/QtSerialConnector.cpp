#ifdef QT_IS_AVAILABLE
#include "QtSerialConnector.h"

QtSerialConnector::QtSerialConnector()
{
    connect(this, &QSerialPort::errorOccurred, this, &QtSerialConnector::on_errorOccurred);
#if defined(__arm__)
    open(presentVoyagers().front());
#endif
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
    QMutexLocker mutexLocker(&m_dataQueueMutex);
    m_dataQueue.append(reinterpret_cast<const char*>(data.data()), data.size());
}

void QtSerialConnector::priorityWrite(const std::vector<uint8_t> &data) {
    write(data);
}

size_t QtSerialConnector::dataAvailable() {
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

bool QtSerialConnector::open(std::string port) {
    if(!QSerialPort::isOpen()) {
        setPortName(QString::fromStdString(port));
        setBaudRate(256000);
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

void QtSerialConnector::process() {
    if(isOpen()) {
        QMutexLocker mutexLocker(&m_dataQueueMutex);
        qint64 bytesWritten = QSerialPort::write(m_dataQueue);
        m_dataQueue.remove(0, bytesWritten);
    }
}

void QtSerialConnector::on_errorOccurred(QSerialPort::SerialPortError error) {
    if (error != QSerialPort::NoError) {
        qWarning() << "DAQ Serial Port error:" <<QSerialPort::errorString();
        close();
        open(presentVoyagers().front());
    }
}

std::vector<std::string> QtSerialConnector::presentVoyagers() {
    std::vector<std::string> voyagers;
    for(auto port: QSerialPortInfo::availablePorts()) {
        if(port.serialNumber() == "01234567")
            voyagers.push_back(port.portName().toStdString());
        if (port.manufacturer() == "Microflown Technologies")
            voyagers.push_back(port.portName().toStdString());
    }
#if defined(__arm__)
    voyagers.push_back("/dev/ttyGS0");
#endif
    return voyagers;
}
#endif

