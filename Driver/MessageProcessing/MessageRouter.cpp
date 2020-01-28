#include "MessageRouter.h"

std::map<uint16_t, std::vector<std::function<void(const google::protobuf::Message &message)>>> MessageRouter::m_routeMap;


void MessageRouter::route(const google::protobuf::Message &message) {
    uint16_t messageHash = MessageHashTable::hashForMessage(message);
    for(auto route: m_routeMap[messageHash]) {
        route(message);
    }
}
