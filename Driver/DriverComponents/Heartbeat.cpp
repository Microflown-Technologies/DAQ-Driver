#include "Heartbeat.h"

Heartbeat::Heartbeat(std::function<void(void)> callback, pMessageProcessor messageProcessor) :
    AbstractDriverComponent(messageProcessor), m_callback(callback)
{
    MessageRouter::addMessageRoute<Beat>(std::bind(&Heartbeat::handleHearthbeatMessage, this, std::placeholders::_1));
    m_hearthbeatTimer.setCallback(std::bind(std::bind(&Heartbeat::hearthbeatTimerTimeout, this)));
    m_hearthbeatTimer.start(1000);
    m_hearthbeatDieTimer.start(3000);
    m_hearthbeatDieTimer.setCallback(std::bind(std::bind(&Heartbeat::hearthbeatDieTimerTimeout, this)));
    m_hearthbeatDieTimer.setSingleShot(true);
    hearthbeatTimerTimeout();
}

void Heartbeat::reset()
{
    m_hearthbeatTimer.start(500);
    m_hearthbeatDieTimer.start(3000);
}

void Heartbeat::process()
{
    m_hearthbeatTimer.process();
    m_hearthbeatDieTimer.process();
}

void Heartbeat::hearthbeatTimerTimeout() {
    Beat beat;
    m_messageProcessor->transmit(beat);
#ifdef QT_IS_AVAILABLE
    emit stillAlive();
#endif
}

void Heartbeat::hearthbeatDieTimerTimeout() {
#ifdef QT_IS_AVAILABLE
    qDebug() << "Heartbeat died, now resetting!";
    emit died();
#else
    std::cout << "Heartbeat died, now resetting!" << std::endl;
#endif
    m_callback();
}

void Heartbeat::handleHearthbeatMessage(const google::protobuf::Message &message) {
    (void)message;
    m_hearthbeatDieTimer.start(3000);
}
