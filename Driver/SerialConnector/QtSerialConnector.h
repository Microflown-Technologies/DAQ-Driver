#ifndef QTSERIALCONNECTOR_H
#define QTSERIALCONNECTOR_H

//Qt framework
#include <QDebug>
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

//Internal headers
#include "AbstractSerialConnector.h"

/**
 * @brief The SerialConnector class automatically connects to the correct serial port
 */
class QtSerialConnector : private QSerialPort, public AbstractSerialConnector
{
    Q_OBJECT
public:
    QtSerialConnector();

    std::vector<uint8_t> read();

    void write(const std::vector<uint8_t> &data);

    void priorityWrite(const std::vector<uint8_t> &data);

    uint64_t dataAvailable() const;

    bool voyagerConnected();

protected:
    /**
     * @brief getSerialPortInfo Gets information about the right serial port
     * @return the found information about the right serial port
     */
    QSerialPortInfo getSerialPortInfo() const;

    /**
     * @brief openConnection Opens connection to the right serial port
     */
    void openConnection();
};

#endif // QTSERIALCONNECTOR_H
