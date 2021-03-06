#ifndef ABSTRACTSOCKETCONNECTOR_H
#define ABSTRACTSOCKETCONNECTOR_H

//STD framework
#include <vector>
#include <memory>
#include <string>
#include <stdint.h>

//Internal headers
#include "Threading/CallbackHandler.h"

/**
 * @brief The AbstractSocketConnector class Defines an generic API for a websocket connection
 */
class AbstractSocketConnector
{
public:
    /**
     * @brief defaultIpAddress Default IP address for the Voyager
     */
    static constexpr const char* defaultIpAddress = "172.20.0.1";

    /**
     * @brief defaultIpAddress Default IP address for the Voyager
     */
    static constexpr const uint16_t defaultPort = 8080;

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
     * @brief isOpen Checks if the connection to the Voyager is open
     * @return returns true if connection is open
     */
    virtual bool isOpen() = 0;

    /**
     * @brief closedCallbackHandler Handler that is invoked when all connections are closed
     * @return associated callbackhandler
     */
    virtual CallbackHandler &closedCallbackHandler() = 0;

    /**
     * @brief openedCallbackHandler Handler that is invoked when a connection is openend
     * @return associated callbackhandler
     */
    virtual CallbackHandler &openedCallbackHandler() = 0;

    /**
     * @brief port Gets the port that is used by this socket connector
     * @return port that is used by this socket connector
     */
    virtual uint16_t port() = 0;


protected:
    CallbackHandler m_closedCallbackHandler;
    CallbackHandler m_openendCallbackHandler;

};
typedef std::shared_ptr<AbstractSocketConnector> pAbstractSocketConnector;

#endif // ABSTRACTSOCKETCONNECTOR_H

