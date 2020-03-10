#ifndef MESSAGEPARSER_H
#define MESSAGEPARSER_H

//STD Framework
#include <vector>
#include <cstring>
#include <stdint.h>
#include <iostream>
#include <memory>


//Internal headers
#include "Message.h"
#include "MessageHashTable.h"
#include "MessageDeserializer.h"

//Protobuf
#include <google/protobuf/message.h>

/**
 * @brief The MessageParser class Parses Protobuf messages from Messages and vice versa
 */
class MessageParser
{
public:
    /**
     * @brief parse parses message to a protobuf message
     * @param message message to serialize
     * @return the parsed protobuff message, returns nullptr if was unable to parse
     */
    static std::shared_ptr<google::protobuf::Message> parse(const Message &message);

    /**
     * @brief parse parses a protobuf message to a message
     * @param message protobuf message to parse
     * @return the parsed message
     */
    static const Message parse(const google::protobuf::Message &message);

};

#endif // MESSAGEPARSER_H
