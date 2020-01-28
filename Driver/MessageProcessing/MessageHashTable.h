#ifndef MESSAGEHASHTABLE_H
#define MESSAGEHASHTABLE_H

// STD framework
#include <map>
#include <string>
#include <cstdint>

// Protobuff
#include <google/protobuf/message.h>

// CRC
#include "CRC.h"

/**
 * @brief The MessageHashTable class Generates hashes for messages names
 */
class MessageHashTable
{
public:

    /**
     * @brief hashForMessageDescriptor Gets hash for message descriptor
     * @param message message to get hash for
     * @return the generated hash
     */
    static uint16_t hashForMessageDescriptor(const google::protobuf::Descriptor *message);

    static uint16_t hashForMessage(const google::protobuf::Message &message);


    /**
     * @brief getDescriptorForHash Gets point to protobuff message Descriptor
     * @param hash hash to get descriptor for
     * @return returns null if no descriptor could be found
     */
    static const google::protobuf::Descriptor* getDescriptorForHash(const uint16_t hash);

    /**
     * @brief hasDescriptorForHash Checks if the hash table contains a descriptor for this hash
     * @param hash hash to check for
     * @return returns true if the hashtable contains a descriptor for this hash
     */
    static bool hasDescriptorForHash(const uint16_t hash);

    template <class T>
    static uint16_t hashForMessageType() {
        return hashForMessageDescriptor(T::descriptor());
    }

    template <class T>
    /**
     * @brief addMessage Adds message to the hash table
     */
    static void addMessage() {
        hashForMessageType<T>();
    }

protected:
    /**
     * @brief calculateHashForString calculate hash for string
     * @param string string to calculate hash for
     * @return calculated hash
     */
    static uint16_t calculateHashForString(const std::string &string);

private:
    static CRC::Table<std::uint16_t, 16> m_crcTable;
    static std::map<const google::protobuf::Descriptor*, uint16_t> m_messageHashes;
};

#endif // MESSAGEHASHTABLE_H
