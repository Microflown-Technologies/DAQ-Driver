#ifndef HEARTBEAT_H
#define HEARTBEAT_H

//Internal headers
#include "AbstractDriverComponent.h"
#include "MessageProcessing/MessageRouter.h"

//Protobuf
#include "Beat.pb.h"
#include <google/protobuf/message.h>

/**
 * @brief The Heartbeat class Make sure both devices are still alive
 */
class Heartbeat : public AbstractDriverComponent
{
public:
    Heartbeat(std::function<void(void)> callback, MessageProcessor &messageProcessor);

    void reset();

    void process();

protected:
    void hearthbeatTimerTimeout();
    void hearthbeatDieTimerTimeout();
    void handleHearthbeatMessage(const google::protobuf::Message &message);

private:
    Timer m_hearthbeatTimer;
    Timer m_hearthbeatDieTimer;
    std::function<void(void)> m_callback;
};

#endif // HEARTBEAT_H
