#ifndef ABSTRACTSERIALCONNECTOR_H
#define ABSTRACTSERIALCONNECTOR_H

//STD framework
#include <vector>
#include <stdint.h>


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
    virtual uint64_t dataAvailable() const = 0;

    /**
     * @brief read Reads pending data (if any) to a buffer
     * @return data that was read
     */
    virtual std::vector<uint8_t> read() = 0;

    /**
     * @brief voyagerConnected Checks if the Voyager is connected
     * @return returns true if the Voyager is connected
     */
    virtual bool voyagerConnected() = 0;

};

#endif // ABSTRACTSERIALCONNECTOR_H
