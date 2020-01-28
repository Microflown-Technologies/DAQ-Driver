#ifndef GENERICSERIALCONNECTOR_H
#define GENERICSERIALCONNECTOR_H

#include <iostream>
//Internal headers
#include "AbstractSerialConnector.h"

//Serial library
#include "rs232/rs232.h"

class GenericSerialConnector : public AbstractSerialConnector
{
public:
    GenericSerialConnector();

    void write(const std::vector<uint8_t> &data);
    void priorityWrite(const std::vector<uint8_t> &data);


    uint64_t dataAvailable() const;

    std::vector<uint8_t> read();

    bool voyagerConnected();
};

#endif // GENERICSERIALCONNECTOR_H
