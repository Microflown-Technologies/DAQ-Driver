#include "MessageAcknowledgement.h"

MessageAcknowledgement::MessageAcknowledgement() :
    m_recievedAcknoledgement(true), m_pendingMessageChanged(timeSinceEpochMillisec())
{
    MessageRouter::addMessageRoute<MessageAck>(std::bind(&MessageAcknowledgement::handleMessageAckRecieved, this, std::placeholders::_1));
}

void MessageAcknowledgement::setPendingMessage(const google::protobuf::Message &message) {
    m_pendingMessageChanged = timeSinceEpochMillisec();
    m_recievedAcknoledgement = false;
    m_pendingMessageDescriptor = message.GetDescriptor();
}

void MessageAcknowledgement::handleMessageAckRecieved(const google::protobuf::Message &message) {
    std::cout << "Ack for message " << message.GetTypeName() << " Pending " << m_pendingMessageDescriptor->name() << std::endl;
    std::cout << timeSinceEpochMillisec() - m_pendingMessageChanged << std::endl;
    if(static_cast<const MessageAck&>(message).messagehash() == MessageHashTable::hashForMessageDescriptor(m_pendingMessageDescriptor)) {
        m_recievedAcknoledgement = true;
    }
}

uint64_t MessageAcknowledgement::timeSinceEpochMillisec() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

bool MessageAcknowledgement::recievedAcknowledgement() const {
    return m_recievedAcknoledgement || (m_pendingMessageChanged + 200 >= timeSinceEpochMillisec());
}
