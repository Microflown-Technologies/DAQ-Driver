#ifndef MESSAGEPROCESSOR_H
#define MESSAGEPROCESSOR_H

//Internal headers
#include "Timer.h"
#include "Message.h"
#include "MessageParser.h"
#include "MessageRouter.h"
#include "MessageHashTable.h"
#include "MessageSerializer.h"
#include "MessageDeserializer.h"
#include "SocketConnector/AbstractSocketConnector.h"

//STD framework
#include <memory>

//Protobuff
#include <MessageAck.pb.h>
#include <google/protobuf/message.h>

/**
 * @brief The MessageProcessor class Handles and processes all incomming and outgoing messages
 */
class MessageProcessor
{
public:
    MessageProcessor(pAbstractSocketConnector socketConnector);

    /**
     * @brief transmit Transmits a message to the Voyager
     * @param message the message to transmit to the Voyager
     */
    void transmit(const google::protobuf::Message &message);

    /**
     * @brief process processes all the messages
     * @details this function needs to be run periodically for the processor to function
     */
    void process();

private:
    Timer m_clearTimer;
    pAbstractSocketConnector m_socketConnector; ///< Serial connection to the Voyager
};
typedef std::shared_ptr<MessageProcessor> pMessageProcessor;
#endif // MESSAGEPROCESSOR_H
