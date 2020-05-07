#ifndef ABSTRACTSOCKETCONNECTOR_H
#define ABSTRACTSOCKETCONNECTOR_H

//STD framework
#include <vector>
#include <memory>
#include <string>
#include <stdint.h>

/**
 * @brief The AbstractSocketConnector class Defines an generic API for a websocket connection
 */
class AbstractSocketConnector
{
public:
    /**
     * @brief sendMessage Sends message to the Voyager through the socket connection
     * @param message message to ben sent
     */
    virtual void sendMessage(const std::vector<uint8_t> &message) = 0;

    /**
     * @brief messagesAvailable Gets the amount of messages available
     * @return the amount of available messages
     */
    virtual size_t messagesAvailable() = 0;

    /**
     * @brief nextMessage Returns the next queued message
     * @return return next message, and empty array if no messages are available
     */
    virtual std::vector<uint8_t> nextMessage() = 0;

    /**
     * @brief process process data
     */
    virtual void process() = 0;
};

#endif // ABSTRACTSOCKETCONNECTOR_H
