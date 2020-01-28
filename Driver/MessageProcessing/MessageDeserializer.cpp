#include "MessageDeserializer.h"

std::queue<Message>     MessageDeserializer::m_incommingMessages; ///< Temporarily stores incomming messages
std::vector<uint8_t>    MessageDeserializer::m_incommingDataBuffer; ///< Temporarily buffers incomming data

void MessageDeserializer::processData(std::vector<uint8_t> data)
{
    //Add data
    m_incommingDataBuffer.insert(m_incommingDataBuffer.end(), data.begin(), data.end());
    if(hasFullMessageReady()) {
        m_incommingMessages.push(takeFullMessageFromBuffer());
    }
}

void MessageDeserializer::clear() {
    m_incommingDataBuffer.clear();
    while(!m_incommingMessages.empty()) m_incommingMessages.pop();
}

bool MessageDeserializer::hasMessagesAvailable() {
    return !m_incommingMessages.empty();
}

const Message MessageDeserializer::nextMessage()
{
    if(hasMessagesAvailable()) {
        Message retrievedMessage = m_incommingMessages.front();
        m_incommingMessages.pop();
        return retrievedMessage;
    } else return Message();
}

bool MessageDeserializer::hasFullMessageReady() {
    if(m_incommingDataBuffer.size() >= sizeof(MessageHeader)) {
        //Get header from incomming data
        MessageHeader messageHeader;
        std::memcpy(&messageHeader, m_incommingDataBuffer.data(), sizeof(messageHeader));
        return (m_incommingDataBuffer.size() >= sizeof(MessageHeader) + messageHeader.messageSize);
    } else return false;
}

MessageHeader MessageDeserializer::frameHeaderFromData(const std::vector<uint8_t> &data) {
    //Get header from incomming data
    MessageHeader messageHeader;
    std::memcpy(&messageHeader, data.data(), sizeof(messageHeader));
    return messageHeader;
}

Message MessageDeserializer::takeFullMessageFromBuffer() {
    //Get header from incomming data
    MessageHeader messageHeader = frameHeaderFromData(m_incommingDataBuffer);

    //Copy data to new array
    std::vector<uint8_t> fullMessage(m_incommingDataBuffer.begin() + sizeof(MessageHeader),
                                     m_incommingDataBuffer.begin() + sizeof(MessageHeader) + messageHeader.messageSize);

    m_incommingDataBuffer.erase(m_incommingDataBuffer.begin(),
                                m_incommingDataBuffer.begin() +
                                messageHeader.messageSize +
                                sizeof(MessageHeader));

    //Create message
    Message message;
    message.header = messageHeader;
    message.data = fullMessage;
    return message;
}
