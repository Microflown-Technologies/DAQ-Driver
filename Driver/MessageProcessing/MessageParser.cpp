#include "MessageParser.h"

std::shared_ptr<google::protobuf::Message> MessageParser::parse(const Message &message) {
    if(MessageHashTable::hasDescriptorForHash(message.header.messageHash)) {
        const google::protobuf::Message *generatedMessage;
        generatedMessage = google::protobuf::MessageFactory::generated_factory()->GetPrototype(MessageHashTable::getDescriptorForHash(message.header.messageHash));
        google::protobuf::Message *newMessage = generatedMessage->New();
        newMessage->ParseFromArray(message.data.data(), static_cast<int>(message.data.size()));
        return std::shared_ptr<google::protobuf::Message>(newMessage);
    } else return std::shared_ptr<google::protobuf::Message>();
}

const Message MessageParser::parse(const google::protobuf::Message &message) {
    Message serializedMessage;
    serializedMessage.header.messageHash = MessageHashTable::hashForMessage(message);
    serializedMessage.header.messageSize  = static_cast<uint32_t>(message.ByteSizeLong());
    serializedMessage.data.resize(message.ByteSizeLong());
    message.SerializePartialToArray(serializedMessage.data.data(), static_cast<int>(serializedMessage.data.size()));
    return serializedMessage;
}
