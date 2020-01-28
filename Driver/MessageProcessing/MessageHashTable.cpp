#include "MessageHashTable.h"

std::map<const google::protobuf::Descriptor*, uint16_t> MessageHashTable::m_messageHashes;
CRC::Table<std::uint16_t, 16> MessageHashTable::m_crcTable(CRC::CRC_16_KERMIT());

uint16_t MessageHashTable::hashForMessageDescriptor(const google::protobuf::Descriptor *messageDescriptor) {
    if(m_messageHashes.count(messageDescriptor)) {
        return m_messageHashes[messageDescriptor];
    } else {
        uint16_t hash = calculateHashForString(messageDescriptor->name());
        m_messageHashes[messageDescriptor] = hash;
        return hashForMessageDescriptor(messageDescriptor);
    }
}

uint16_t MessageHashTable::hashForMessage(const google::protobuf::Message &message) {
    return hashForMessageDescriptor(message.GetDescriptor());
}

const google::protobuf::Descriptor *MessageHashTable::getDescriptorForHash(const uint16_t hash) {
    for(auto iterator = m_messageHashes.begin(); iterator != m_messageHashes.end(); ++iterator) {
        if (iterator->second == hash) return iterator->first;
    }
    return nullptr;
}

bool MessageHashTable::hasDescriptorForHash(const uint16_t hash) {
    for(auto iterator = m_messageHashes.begin(); iterator != m_messageHashes.end(); ++iterator) {
        if (iterator->second == hash) return true;
    }
    return false;
}

uint16_t MessageHashTable::calculateHashForString(const std::string &string) {
    return CRC::Calculate(string.data(), string.size(), m_crcTable);
}

