#ifndef ABSTRACTSERIALCONNECTOR_H
#define ABSTRACTSERIALCONNECTOR_H

//STD framework
#include <vector>
#include <string>
#include <stdint.h>

/**
 * @brief The AbstractSerialConnector class Defines an generic API for a serial port connection
 */
class AbstractSerialConnector
{
public:
    /**
     * @brief write Writes data to the Voyager through the serial connection
     * @param data data to write
     */
    virtual void write(const std::vector<uint8_t> &data) = 0;

    /**
     * @brief priorityWrite Sames as write but immediatly flushes the buffers
     * @param data data to write
     */
    virtual void priorityWrite(const std::vector<uint8_t> &data) = 0;

    /**
     * @brief dataAvailable Checks the amount of bytes available
     * @return the amount of bytes available
     */
    virtual size_t dataAvailable() = 0;

    /**
     * @brief read Reads pending data (if any) to a buffer
     * @return data that was read
     */
    virtual std::vector<uint8_t> read() = 0;

    /**
     * @brief open Opens the serial connection to the Voyager
     * @param port specified which port to connect to
     * @return returns true on succes
     */
    virtual bool open(std::string port) = 0;

    /**
     * @brief close Closes the serial connection to the Voyager
     */
    virtual void close() = 0;


    /**
     * @brief isOpen Checks if there is an connection to the Voyager
     * @return returns true if there is an connector to a Voyager
     */
    virtual bool isOpen() = 0;

    /**
     * @brief presentVoyagers Enumerates all connected Voyager
     * @return returns list of serial ports that are connected to the Voyager
     */
    virtual std::vector<std::string> presentVoyagers() = 0;

};

#endif // ABSTRACTSERIALCONNECTOR_H
