#ifndef MESSAGEROUTER_H
#define MESSAGEROUTER_H

//STD framework
#include <map>
#include <cstdint>
#include <functional>
#include <memory>


//Internal headers
#include "MessageHashTable.h"

//Protobuf
#include <google/protobuf/message.h>

/**
 * @brief The MessageRouter class Routes messages to places in your code
 */
class MessageRouter
{
public:

    /**
     * @brief route Routes a message
     * @param message message to route
     */
    static void route(const google::protobuf::Message &message);

    template <class T>
    static void addMessageRoute(std::function<void(const google::protobuf::Message &message)> route) {
        uint16_t messageHash = MessageHashTable::hashForMessageType<T>();
        m_routeMap[messageHash].emplace_back(route);
    }
    /**
     * @brief clearAllRoutes Clear all message routes
     */
    static void clearAllRoutes() {
        m_routeMap.clear();
    }
    
private:
    static std::map<uint16_t, std::vector<std::function<void(const google::protobuf::Message &message)>>> m_routeMap;
};

#endif // MESSAGEROUTER_H
