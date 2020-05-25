#include "DeviceControl.h"

DeviceControl::DeviceControl(pMessageProcessor messageProcessor) : AbstractDriverComponent(messageProcessor), m_hasControl(false) {
  MessageRouter::addMessageRoute<GrabControl>(std::bind(&DeviceControl::handleGrabControlRecieved, this, std::placeholders::_1));
  MessageRouter::addMessageRoute<ReleaseControl>(std::bind(&DeviceControl::handleReleaseControlRecieved, this, std::placeholders::_1));
  MessageRouter::addMessageRoute<Reset>(std::bind(&DeviceControl::handleResetRecieved, this, std::placeholders::_1));
}

void DeviceControl::reset() {
    m_hasControl = false;
#ifdef QT_IS_AVAILABLE
    emit releasedControl();
    emit hasControlChanged(m_hasControl);
#endif
}

void DeviceControl::takeControl() {
    m_messageProcessor->transmit(GrabControl());
    m_hasControl = true;
}

void DeviceControl::releaseControl() {
    m_messageProcessor->transmit(ReleaseControl());
    m_hasControl = false;
}

bool DeviceControl::hasControl() const {
    return m_hasControl;
}

bool DeviceControl::removeResetCallback(std::shared_ptr<std::function<void ()> > callback) {
    return m_resetCallbackHandler.removeCallback(callback);
}

void DeviceControl::handleResetRecieved(const google::protobuf::Message &message)
{
    (void) message;
    m_resetCallbackHandler.invokeCallbacks();
#ifdef QT_IS_AVAILABLE
    emit resetRequested();
#endif
}

void DeviceControl::handleGrabControlRecieved(const google::protobuf::Message &message)
{
    (void) message;
    m_hasControl = true;
    m_grabbedControlCallbackHandler.invokeCallbacks();
#ifdef QT_IS_AVAILABLE
    emit takenControl();
    emit hasControlChanged(m_hasControl);
#endif
}

void DeviceControl::handleReleaseControlRecieved(const google::protobuf::Message &message)
{
    (void) message;
    m_hasControl = false;
    m_releasedControlCallbackHandler.invokeCallbacks();
#ifdef QT_IS_AVAILABLE
    emit releasedControl();
    emit hasControlChanged(m_hasControl);
#endif
}

std::shared_ptr<std::function<void (void)> > DeviceControl::addReleasedControlCallback(const std::function<void ()> &releasedControlCallback) {
    return m_releasedControlCallbackHandler.addCallback(releasedControlCallback);
}

std::shared_ptr<std::function<void ()> > DeviceControl::addGrabbedControlCallback(const std::function<void ()> &grabbedControlCallback) {
    return m_grabbedControlCallbackHandler.addCallback(grabbedControlCallback);
}

bool DeviceControl::removeReleasedControlCallback(std::shared_ptr<std::function<void ()> > callback) {
    return m_releasedControlCallbackHandler.removeCallback(callback);
}

std::shared_ptr<std::function<void ()> > DeviceControl::addResetCallback(const std::function<void ()> &resetCallback) {
    return m_resetCallbackHandler.addCallback(resetCallback);
}

bool DeviceControl::removeGrabbedControlCallback(std::shared_ptr<std::function<void ()> > callback) {
    return m_grabbedControlCallbackHandler.removeCallback(callback);
}


