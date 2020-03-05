#ifdef QT_IS_AVAILABLE

#ifndef QTSERIALCONNECTOR_H
#define QTSERIALCONNECTOR_H

//Qt framework
#include <QDebug>
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

//Std framework
#include <string>
#include <vector>

//Internal headers
#include "AbstractSerialConnector.h"

/**
 * @brief The SerialConnector class automatically connects to the correct serial port
 */
class QtSerialConnector : public QSerialPort, public AbstractSerialConnector
{
    Q_OBJECT
public:
    QtSerialConnector();

    std::vector<uint8_t> read() override;

    void write(const std::vector<uint8_t> &data) override;

    void priorityWrite(const std::vector<uint8_t> &data) override;

    size_t dataAvailable() override;

    bool isOpen() override;

    void close() override;

    std::vector<std::string> presentVoyagers() override;

    bool open(std::string port) override;

private slots:
    void on_errorOccurred(QSerialPort::SerialPortError error);
protected:

};

#endif // QTSERIALCONNECTOR_H

#endif
