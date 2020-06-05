#ifndef DEVICESTATUS_H
#define DEVICESTATUS_H

//Std framework
#include <memory>

//Internal headers
#include "AbstractDriverComponent.h"
#include "Threading/CallbackHandler.h"

//Protobuff messages
#include "DeviceStatusStructure.pb.h"

/**
 * @brief The DeviceStatus class can be used to retrieve the status of the Voyager such as temperature or battery charge level
 */
class DeviceStatus : public AbstractDriverComponent
{
#ifdef QT_IS_AVAILABLE
    Q_OBJECT
#endif
public:
    DeviceStatus(pMessageProcessor messageProcessor);

    /**
     * @brief remoteDeviceStatus Retrieves the current remote device status
     * @return current remote device status
     */
    DeviceStatusStructure remoteDeviceStatus() const;

    /**
     * @brief sendStatus Sends device status to other remote device
     * @param status status to send
     */
    void sendStatus(const DeviceStatusStructure status);

    std::shared_ptr<std::function<void ()> > addremoteDeviceStatusUpdated(const std::function<void ()> &newBufferCallback);
    bool removeRemoteDeviceStatusUpdated(std::shared_ptr<std::function<void(void)>> callback);

    virtual void reset() override;

#ifdef QT_IS_AVAILABLE
signals:
    /**
     * @brief remoteDeviceUpdated is emitted when new deviceinfo is recieved from the remote device
     */
    void remoteDeviceUpdated();
#endif

protected:
    void handleDeviceStatusRecieved(const google::protobuf::Message &message);


private:
    DeviceStatusStructure m_remoteDeviceStatus;
    CallbackHandler m_remoteDeviceStatusUpdatedCallbackHandler;
};
typedef std::shared_ptr<DeviceStatus> pDeviceStatus;

#endif // DEVICESTATUS_H
