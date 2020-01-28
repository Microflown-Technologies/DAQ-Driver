#include "MessageSerializer.h"

const std::vector<uint8_t> MessageSerializer::serialize(const Message &message) {
    std::vector<uint8_t> serializedMessage(sizeof(MessageHeader) + message.data.size());
    std::memcpy(serializedMessage.data(), &message.header, sizeof(MessageHeader));
    std::memcpy(&serializedMessage[sizeof(MessageHeader)], message.data.data(), message.data.size());
    return serializedMessage;
}
