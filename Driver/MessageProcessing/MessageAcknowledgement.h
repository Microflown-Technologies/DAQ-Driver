#ifndef MESSAGEACKNOWLEDGEMENT_H
#define MESSAGEACKNOWLEDGEMENT_H

//STD library
#include <memory>
#include <chrono>

//Internal headers
#include "MessageRouter.h"
#include "MessageHashTable.h"

//Protobuf
#include <google/protobuf/message.h>
#include "MessageAck.pb.h"


/**
 * @brief The MessageAcknowledgement class Handled the flow of messages and their acknowledgement
 */
class MessageAcknowledgement
{
public:
    MessageAcknowledgement();

    /**
     * @brief setPendingMessage Set message to wait for
     * @param message message to wait for
     */
    void setPendingMessage(const google::protobuf::Message &message);

    /**
     * @brief recievedAcknowledgement Returns if a Acknoledgement message was sent for m_pendingMessageDescriptor
     * @return returns true if ac
     */
    bool recievedAcknowledgement() const;

protected:
    void handleMessageAckRecieved(const google::protobuf::Message &message);
    static uint64_t timeSinceEpochMillisec();

private:
    bool m_recievedAcknoledgement;
    uint64_t m_pendingMessageChanged;
    const google::protobuf::Descriptor *m_pendingMessageDescriptor;
};

#endif // MESSAGEACKNOWLEDGEMENT_H
