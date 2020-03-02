#ifndef DEVICEINFO_H
#define DEVICEINFO_H

//Std framework
#include <mutex>
#include <string>

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
    DeviceInfo(MessageProcessor &messageProcessor);

    /**
     * @brief refresh Updates the deviceInfo
     */
    void refresh();

    DeviceInfoStructure localDeviceInfo();
    void setLocalDeviceInfo(const DeviceInfoStructure &localDeviceInfo);

    DeviceInfoStructure remoteDeviceInfo();

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

};

#endif // DEVICEINFO_H
