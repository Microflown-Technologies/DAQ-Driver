#include "DeviceInfo.h"

DeviceInfo::DeviceInfo(pMessageProcessor messageProcessor) : AbstractDriverComponent(messageProcessor)
{
    // Set callbacks
    MessageRouter::addMessageRoute<DeviceInfoResponse>(std::bind(&DeviceInfo::handleDeviceInfoResponseRecieved, this, std::placeholders::_1));
    MessageRouter::addMessageRoute<DeviceInfoRequest>(std::bind(&DeviceInfo::handleDeviceInfoRequestRecieved, this, std::placeholders::_1));
    m_localDeviceInfo.set_modelname("");
    m_localDeviceInfo.set_devicename("");
    m_localDeviceInfo.set_drivername("VOYAGERV1");
    m_localDeviceInfo.set_driverversion(DRIVER_VERSION);
    m_localDeviceInfo.set_softwareversion("");
    m_localDeviceInfo.set_deviceserial("");
}

void DeviceInfo::refresh() {
    m_messageProcessor->transmit(DeviceInfoRequest());
}

DeviceInfoStructure DeviceInfo::localDeviceInfo() {
    std::lock_guard<std::mutex> mutexLocker(m_localDeviceInfoMutex);
    return m_localDeviceInfo;
}

void DeviceInfo::setLocalDeviceInfo(const DeviceInfoStructure &localDeviceInfo) {
    std::lock_guard<std::mutex> mutexLocker(m_localDeviceInfoMutex);
    m_localDeviceInfo = localDeviceInfo;
}

DeviceInfoStructure DeviceInfo::remoteDeviceInfo() {
    std::lock_guard<std::mutex> mutexLocker(m_remoteDeviceInfoMutex);
    return m_remoteDeviceInfo;
}

std::shared_ptr<std::function<void ()> > DeviceInfo::addRemoteInfoRecievedCallback(const std::function<void ()> &newBufferCallback) {
    return m_remoteInfoRecievedCallbackHandler.addCallback(newBufferCallback);
}

bool DeviceInfo::removeRemoteInfoRecievedCallback(std::shared_ptr<std::function<void ()> > callback) {
    return m_remoteInfoRecievedCallbackHandler.removeCallback(callback);
}

void DeviceInfo::reset()
{
    refresh();
}

void DeviceInfo::handleDeviceInfoRequestRecieved(const google::protobuf::Message &message) {
    (void) message;
    std::lock_guard<std::mutex> mutexLocker(m_remoteDeviceInfoMutex);
    DeviceInfoResponse deviceInfoResponse = DeviceInfoResponse();
    deviceInfoResponse.set_allocated_deviceinfo(new DeviceInfoStructure(m_localDeviceInfo));
    m_messageProcessor->transmit(deviceInfoResponse);
}

void DeviceInfo::handleDeviceInfoResponseRecieved(const google::protobuf::Message &message) {
    //Copy DeviceInfoResponse
    DeviceInfoResponse deviceInfoResponse;
    deviceInfoResponse.CopyFrom(message);
    setRemoteDeviceInfo(deviceInfoResponse.deviceinfo());
    m_remoteInfoRecievedCallbackHandler.invokeCallbacks();
}

void DeviceInfo::setRemoteDeviceInfo(const DeviceInfoStructure &remoteDeviceInfo) {
    std::lock_guard<std::mutex> mutexLocker(m_remoteDeviceInfoMutex);
    m_remoteDeviceInfo = remoteDeviceInfo;
}
