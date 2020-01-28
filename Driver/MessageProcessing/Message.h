#ifndef MESSAGE_H
#define MESSAGE_H

#include <vector>
#include <stdint.h>

struct MessageHeader {
    uint16_t messageHash;
    uint32_t messageSize;
};
struct Message {
    MessageHeader header;
    std::vector<uint8_t> data;
};

#endif // MESSAGE_H
