#include "MessageProcessor.h"

MessageProcessor::MessageProcessor(AbstractSerialConnector &serialConnector) : m_serialConnector(serialConnector)
{
    m_clearTimer.setCallback(MessageDeserializer::clear);
}

void MessageProcessor::transmit(const google::protobuf::Message &message) {
    const Message parsedMessage = MessageParser::parse(message);
    const std::vector<uint8_t> serializedMessage = MessageSerializer::serialize(parsedMessage);
    if(message.GetDescriptor() != MessageAck::descriptor()) {
        m_messageAcknowledgement.setPendingMessage(message);
        m_serialConnector.write(serializedMessage);
        while(!m_messageAcknowledgement.recievedAcknowledgement()) {
            process();
        }
    } else {
        m_serialConnector.write(serializedMessage);
    }
}

void MessageProcessor::process() {
    if(m_serialConnector.dataAvailable()) {
        m_clearTimer.start();
        MessageDeserializer::processData(m_serialConnector.read());
        while (MessageDeserializer::hasMessagesAvailable()) {
            Message message = MessageDeserializer::nextMessage();
            std::shared_ptr<google::protobuf::Message> protobuffMessage = MessageParser::parse(message);
            if(protobuffMessage) {
                MessageRouter::route(*protobuffMessage);
                //Transmit acknoledgement
                if(protobuffMessage->GetDescriptor() != MessageAck::descriptor()) {
                    MessageAck messageAcknowledgement;
                    messageAcknowledgement.set_messagehash(MessageHashTable::hashForMessage(*protobuffMessage));
                    transmit(messageAcknowledgement);
                }
            }
        }
    }
    m_clearTimer.process();
}
