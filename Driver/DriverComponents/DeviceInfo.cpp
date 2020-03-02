#include "DeviceInfo.h"

DeviceInfo::DeviceInfo(MessageProcessor &messageProcessor) : AbstractDriverComponent(messageProcessor)
{
    m_localDeviceInfo.set_modelname("Computer");
    m_localDeviceInfo.set_devicename("Computer");
    m_localDeviceInfo.set_drivername("VOYAGERV1");
    m_localDeviceInfo.set_driverversion(DRIVER_VERSION);
    m_localDeviceInfo.set_softwareversion("");
    m_localDeviceInfo.set_deviceserial("");
}

void DeviceInfo::refresh() {
    m_messageProcessor.transmit(DeviceInfoRequest());
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

void DeviceInfo::reset()
{
    refresh();
}

void DeviceInfo::handleDeviceInfoRequestRecieved(const google::protobuf::Message &message) {
    (void) message;
    std::lock_guard<std::mutex> mutexLocker(m_remoteDeviceInfoMutex);
    DeviceInfoResponse deviceInfoResponse;
    deviceInfoResponse.set_allocated_deviceinfo(&m_localDeviceInfo);
    m_messageProcessor.transmit(deviceInfoResponse);
}

void DeviceInfo::handleDeviceInfoResponseRecieved(const google::protobuf::Message &message) {
    //Copy DeviceInfoResponse
    DeviceInfoResponse deviceInfoResponse;
    deviceInfoResponse.CopyFrom(message);
    setRemoteDeviceInfo(deviceInfoResponse.deviceinfo());
}

void DeviceInfo::setRemoteDeviceInfo(const DeviceInfoStructure &remoteDeviceInfo) {
    std::lock_guard<std::mutex> mutexLocker(m_remoteDeviceInfoMutex);
    m_remoteDeviceInfo = remoteDeviceInfo;
}
