#ifdef QT_IS_AVAILABLE

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
class QtSerialConnector : public QSerialPort, public AbstractSerialConnector
{
    Q_OBJECT
public:
    QtSerialConnector();

    std::vector<uint8_t> read() override;

    void write(const std::vector<uint8_t> &data) override;

    void priorityWrite(const std::vector<uint8_t> &data) override;

    size_t dataAvailable() const override;

    bool isOpen() override;

    void close() override;

    bool voyagerConnected() override;

    bool open();

private slots:
    void on_errorOccurred(QSerialPort::SerialPortError error);
protected:
    /**
     * @brief getSerialPortInfo Gets information about the right serial port
     * @return the found information about the right serial port
     */
    QSerialPortInfo getSerialPortInfo() const;

    /**
     * @brief openConnection Opens connection to the right serial port
     */
};

#endif // QTSERIALCONNECTOR_H

#endif
