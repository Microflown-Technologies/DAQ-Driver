#ifndef MESSAGE_H
#define MESSAGE_H

#include <vector>
#include <stdint.h>

/**
 * @brief The MessageHeader struct Defines the binary format of a message header
 */
typedef struct {
    uint16_t messageHash;
    uint32_t messageSize;
} MessageHeader;
/**
 * @brief The Message struct Defines the binary format of a message
 */
typedef struct {
    MessageHeader header;
    std::vector<uint8_t> data;
} Message;

#endif // MESSAGE_H
