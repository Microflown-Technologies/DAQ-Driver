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
    const std::lock_guard<std::mutex> gaurd(m_resetCallbackMutex);
    for(auto callback: m_resetCallback) {
        callback();
    }
#ifdef QT_IS_AVAILABLE
    emit resetRequested();
#endif
}

void DeviceControl::handleGrabControlRecieved(const google::protobuf::Message &message)
{
    (void) message;
    m_hasControl = true;
    const std::lock_guard<std::mutex> gaurd(m_grabbedControlCallbackMutex);
    for(auto callback: m_grabbedControlCallback) {
        callback();
    }
#ifdef QT_IS_AVAILABLE
    emit takenControl();
    emit hasControlChanged(m_hasControl);
#endif
}

void DeviceControl::handleReleaseControlRecieved(const google::protobuf::Message &message)
{
    (void) message;
    m_hasControl = false;
    const std::lock_guard<std::mutex> gaurd(m_releasedControlCallbackMutex);
    for(auto callback: m_releasedControlCallback) {
        callback();
    }
#ifdef QT_IS_AVAILABLE
    emit releasedControl();
    emit hasControlChanged(m_hasControl);
#endif
}

void DeviceControl::addReleasedControlCallback(const std::function<void ()> &releasedControlCallback) {
    const std::lock_guard<std::mutex> gaurd(m_releasedControlCallbackMutex);
    m_releasedControlCallback.push_back(releasedControlCallback);
}

void DeviceControl::addGrabbedControlCallback(const std::function<void ()> &grabbedControlCallback) {
    const std::lock_guard<std::mutex> gaurd(m_grabbedControlCallbackMutex);
    m_grabbedControlCallback.push_back(grabbedControlCallback);
}

void DeviceControl::addResetCallback(const std::function<void ()> &resetCallback) {
    const std::lock_guard<std::mutex> gaurd(m_resetCallbackMutex);
    m_resetCallback.push_back(resetCallback);
}


