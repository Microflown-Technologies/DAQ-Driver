#ifndef DEVICEINFO_H
#define DEVICEINFO_H

//Std framework
#include <mutex>
#include <string>
#include <memory>

//Internal headers
#include "DriverInfo.h"
#include "AbstractDriverComponent.h"

//Protobuf Messages
#include "DeviceInfoRequest.pb.h"
#include "DeviceInfoStructure.pb.h"
#include "DeviceInfoResponse.pb.h"

/**
 * @brief The DeviceInfo class Provides information about the Voyager
 */
class DeviceInfo : public AbstractDriverComponent
{
#ifdef QT_IS_AVAILABLE
    Q_OBJECT
#endif
public:
    DeviceInfo(pMessageProcessor messageProcessor);

    /**
     * @brief refresh Updates the remote  deviceInfo
     */
    void refresh();

    /**
     * @brief localDeviceInfo Gets the information of the device this code is running on
     * @return information about device this code is running on
     */
    DeviceInfoStructure localDeviceInfo();
    void setLocalDeviceInfo(const DeviceInfoStructure &localDeviceInfo);

    /**
     * @brief remoteDeviceInfo Gets the information of the device that you are connected to
     * @return deviceinfo of remote device
     */
    DeviceInfoStructure remoteDeviceInfo();

    std::shared_ptr<std::function<void ()> > addRemoteInfoRecievedCallback(const std::function<void ()> &newBufferCallback);
    bool removeRemoteInfoRecievedCallback(std::shared_ptr<std::function<void(void)>> callback);

    void reset() override;

protected:
    void setRemoteDeviceInfo(const DeviceInfoStructure &remoteDeviceInfo);
    void handleDeviceInfoRequestRecieved(const google::protobuf::Message &message);
    void handleDeviceInfoResponseRecieved(const google::protobuf::Message &message);

private:
    std::mutex m_localDeviceInfoMutex; ///< Mutex for locking device info of local device
    std::mutex m_remoteDeviceInfoMutex; ///< Mutex for locking device info of remote device
    DeviceInfoStructure m_localDeviceInfo; ///< Device Information of local device
    DeviceInfoStructure m_remoteDeviceInfo; ///< Device information of remote device
    CallbackHandler m_remoteInfoRecievedCallbackHandler;

};
typedef std::shared_ptr<DeviceInfo> pDeviceInfo;
#endif // DEVICEINFO_H
