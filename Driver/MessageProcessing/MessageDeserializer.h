#ifndef MESSAGEDESERIALIZER_H
#define MESSAGEDESERIALIZER_H

//STD Framework
#include <queue>
#include <vector>
#include <cstring>
#include <stdint.h>
#include <iostream>

//Internal headers
#include "Timer.h"
#include "Message.h"

/**
 * @brief The MessageDeserializer class Deserializes a binary stream to Messages
 */
class MessageDeserializer
{
public:
    /**
     * @brief processData Processes incomming data
     * @param data incomming data to process
     */
    static void processData(std::vector<uint8_t> data);

    /**
     * @brief clear Clears all internal buffers
     */
    static void clear();

    /**
     * @brief hasMessagesAvailable Check if any incomming messages are available
     * @return returns true if messages are available
     */
    static bool hasMessagesAvailable();

    /**
     * @brief nextMessage Gets next message (if any) from m_incommingMessage
     * @return the retieved message
     */
    static const Message nextMessage();

protected:
    /**
     * @brief hasFullMessageReady Checks if a full message can be read from m_incommingDataBuffer
     * @return returns true if a full message can be read from m_incommingDataBuffer
     */
    static bool hasFullMessageReady();

    /**
     * @brief frameHeaderFromData Get header from frame data
     * @param data data to read header from
     * @return the read header
     */
    static MessageHeader frameHeaderFromData(const std::vector<uint8_t> &data);

    /**
     * @brief takeFullMessageFromBuffer Gets a full message from the m_incommingDataBuffer
     * @return the taken message
     */
    static Message takeFullMessageFromBuffer();

private:
    static std::queue<Message>     m_incommingMessages; ///< Temporarily stores incomming messages
    static std::vector<uint8_t>     m_incommingDataBuffer; ///< Temporarily buffers incomming data
};

#endif // MESSAGEDESERIALIZER_H
