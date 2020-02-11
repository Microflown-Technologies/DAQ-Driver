#include "DeviceControl.h"

DeviceControl::DeviceControl(MessageProcessor &messageProcessor) : AbstractDriverComponent(messageProcessor), m_hasControl(false) {
  MessageRouter::addMessageRoute<GrabControl>(std::bind(&DeviceControl::handleGrabControlRecieved, this, std::placeholders::_1));
  MessageRouter::addMessageRoute<ReleaseControl>(std::bind(&DeviceControl::handleReleaseControlRecieved, this, std::placeholders::_1));
  MessageRouter::addMessageRoute<Reset>(std::bind(&DeviceControl::handleResetRecieved, this, std::placeholders::_1));
}

void DeviceControl::reset() {
    m_messageProcessor.transmit(Reset());
    m_hasControl = false;
#ifdef QT_IS_AVAILABLE
    emit releasedControl();
    emit hasControlChanged(m_hasControl);
#endif
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
    if(m_resetCallback) m_resetCallback();
#ifdef QT_IS_AVAILABLE
    emit resetRequested();
#endif
}

void DeviceControl::handleGrabControlRecieved(const google::protobuf::Message &message)
{
    (void) message;
    m_hasControl = true;
    if(m_grabbedControlCallback) m_grabbedControlCallback();
#ifdef QT_IS_AVAILABLE
    emit takenControl();
    emit hasControlChanged(m_hasControl);
#endif
}

void DeviceControl::handleReleaseControlRecieved(const google::protobuf::Message &message)
{
    (void) message;
    m_hasControl = false;
    if(m_releasedControlCallback) m_releasedControlCallback();
#ifdef QT_IS_AVAILABLE
    emit releasedControl();
    emit hasControlChanged(m_hasControl);
#endif
}

void DeviceControl::setReleasedControlCallback(const std::function<void ()> &releasedControlCallback)
{
    m_releasedControlCallback = releasedControlCallback;
}

void DeviceControl::setGrabbedControlCallback(const std::function<void ()> &grabbedControlCallback)
{
    m_grabbedControlCallback = grabbedControlCallback;
}

void DeviceControl::setResetCallback(const std::function<void ()> &resetCallback)
{
    m_resetCallback = resetCallback;
}


