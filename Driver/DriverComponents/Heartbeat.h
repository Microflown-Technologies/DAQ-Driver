#ifndef HEARTBEAT_H
#define HEARTBEAT_H

//Std framework
#include <memory>

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
#ifdef QT_IS_AVAILABLE
    Q_OBJECT
#endif
public:
    Heartbeat(std::function<void(void)> callback, pMessageProcessor messageProcessor);

    void reset();

    void process();

#ifdef QT_IS_AVAILABLE
signals:
    void died();
    void stillAlive();
#endif

protected:
    void hearthbeatTimerTimeout();
    void hearthbeatDieTimerTimeout();
    void handleHearthbeatMessage(const google::protobuf::Message &message);

private:
    Timer m_hearthbeatTimer;
    Timer m_hearthbeatDieTimer;
    std::function<void(void)> m_callback;
};
typedef std::shared_ptr<Heartbeat> pHeartbeat;
#endif // HEARTBEAT_H
