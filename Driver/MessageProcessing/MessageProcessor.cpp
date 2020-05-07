#include "MessageProcessor.h"

MessageProcessor::MessageProcessor(pAbstractSocketConnector socketConnector) : m_socketConnector(socketConnector)
{
    m_clearTimer.setCallback(MessageDeserializer::clear);
}

void MessageProcessor::transmit(const google::protobuf::Message &message) {
    const Message parsedMessage = MessageParser::parse(message);
    const std::vector<uint8_t> serializedMessage = MessageSerializer::serialize(parsedMessage);
    m_socketConnector->sendMessage(serializedMessage);
}

void MessageProcessor::process() {
    while(m_socketConnector->messagesAvailable()) {
        MessageDeserializer::processData(m_socketConnector->nextMessage());
        while (MessageDeserializer::hasMessagesAvailable()) {
            m_clearTimer.start(1000);
            Message message = MessageDeserializer::nextMessage();
            std::shared_ptr<google::protobuf::Message> protobuffMessage = MessageParser::parse(message);
            if(protobuffMessage) {
                MessageRouter::route(*protobuffMessage);
            }
        }
    }
    m_clearTimer.process();
}
