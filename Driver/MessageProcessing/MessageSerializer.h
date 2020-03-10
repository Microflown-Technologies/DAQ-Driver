#ifndef MESSAGESERIALIZER_H
#define MESSAGESERIALIZER_H

//STD framework
#include <queue>
#include <vector>
#include <memory>
#include <cstring>
#include <stdint.h>

//Internal headers
#include "Message.h"

/**
 * @brief The MessageSerializer class Serializes Messages to a binary stream
 */
class MessageSerializer
{
public:

    /**
     * @brief serialize Serializes a message
     * @param message the message to serialize
     * @return serialized message
     */
    static const std::vector<uint8_t> serialize(const Message &message);
};

#endif // MESSAGESERIALIZER_H
