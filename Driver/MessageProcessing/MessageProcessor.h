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
#include "MessageAcknowledgement.h"
#include "SerialConnector/AbstractSerialConnector.h"

//Protobuff
#include <MessageAck.pb.h>
#include <google/protobuf/message.h>

/**
 * @brief The MessageProcessor class Handles and processes all incomming and outgoing messages
 */
class MessageAcknowledgement;
class MessageProcessor
{
public:
    MessageProcessor(AbstractSerialConnector &serialConnector);

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
    AbstractSerialConnector &m_serialConnector; ///< Serial connection to the Voyager
    MessageAcknowledgement m_messageAcknowledgement; ///< Handles acknowledgement of messages
};

#endif // MESSAGEPROCESSOR_H
