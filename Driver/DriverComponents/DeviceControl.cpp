#include "DeviceControl.h"

DeviceControl::DeviceControl(MessageProcessor &messageProcessor) : AbstractDriverComponent(messageProcessor), m_hasControl(false) {
  MessageRouter::addMessageRoute<GrabControl>(std::bind(&DeviceControl::handleGrabControlRecieved, this, std::placeholders::_1));
  MessageRouter::addMessageRoute<ReleaseControl>(std::bind(&DeviceControl::handleReleaseControlRecieved, this, std::placeholders::_1));
  MessageRouter::addMessageRoute<Reset>(std::bind(&DeviceControl::handleResetRecieved, this, std::placeholders::_1));
}

void DeviceControl::reset() {
    m_messageProcessor.transmit(Reset());
    m_hasControl = false;
}

void DeviceControl::takeControl() {
    m_messageProcessor.transmit(GrabControl());
    m_hasControl = true;
}

void DeviceControl::releaseControl() {
    m_messageProcessor.transmit(ReleaseControl());
    m_hasControl = false;
}

bool DeviceControl::hasControl() const {
    return m_hasControl;
}

void DeviceControl::handleResetRecieved(const google::protobuf::Message &message)
{
    (void) message;
#ifdef QT_IS_AVAILABLE
    emit resetRequested();
#endif
}

void DeviceControl::handleGrabControlRecieved(const google::protobuf::Message &message)
{
    (void) message;
    m_hasControl = true;
#ifdef QT_IS_AVAILABLE
    emit takenControl();
    emit hasControlChanged(m_hasControl);
#endif
}

void DeviceControl::handleReleaseControlRecieved(const google::protobuf::Message &message)
{
    (void) message;
    m_hasControl = false;
#ifdef QT_IS_AVAILABLE
    emit releasedControl();
    emit hasControlChanged(m_hasControl);
#endif
}

void DeviceControl::setResetCallback(const std::function<void ()> &resetCallback)
{
    m_resetCallback = resetCallback;
}


