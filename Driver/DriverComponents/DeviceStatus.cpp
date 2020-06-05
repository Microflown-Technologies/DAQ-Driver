#include "DeviceStatus.h"

DeviceStatus::DeviceStatus(pMessageProcessor messageProcessor) : AbstractDriverComponent(messageProcessor)
{
    MessageRouter::addMessageRoute<DeviceStatusStructure>(std::bind(&DeviceStatus::handleDeviceStatusRecieved, this, std::placeholders::_1));
}

DeviceStatusStructure DeviceStatus::remoteDeviceStatus() const {
    return m_remoteDeviceStatus;
}

void DeviceStatus::sendStatus(const DeviceStatusStructure status) {
    m_messageProcessor->transmit(status);
}

std::shared_ptr<std::function<void ()> > DeviceStatus::addremoteDeviceStatusUpdated(const std::function<void ()> &newBufferCallback){
    return m_remoteDeviceStatusUpdatedCallbackHandler.addCallback(newBufferCallback);
}

bool DeviceStatus::removeRemoteDeviceStatusUpdated(std::shared_ptr<std::function<void ()> > callback) {
    return m_remoteDeviceStatusUpdatedCallbackHandler.removeCallback(callback);
}

void DeviceStatus::reset()
{

}

void DeviceStatus::handleDeviceStatusRecieved(const google::protobuf::Message &message)
{
    m_remoteDeviceStatus = dynamic_cast<const DeviceStatusStructure&>(message);
    m_remoteDeviceStatusUpdatedCallbackHandler.invokeCallbacks();
#ifdef QT_IS_AVAILABLE
    emit remoteDeviceUpdated();
#endif
}
