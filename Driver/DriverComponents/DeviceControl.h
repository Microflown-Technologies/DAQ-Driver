#ifndef DEVICECONTROL_H
#define DEVICECONTROL_H

//Std framework
#include <atomic>
#include <memory>

//Internal headers
#include "AbstractDriverComponent.h"
#include "Threading/CallbackHandler.h"

//Protobuff Messages
#include "Reset.pb.h"
#include "Reboot.pb.h"
#include "Shutdown.pb.h"
#include "GrabControl.pb.h"
#include "ReleaseControl.pb.h"

/**
 * @brief The DeviceControl class Controls the Voyager
 */
class DeviceControl : public AbstractDriverComponent
{
#ifdef QT_IS_AVAILABLE
    Q_OBJECT
#endif
public:
    DeviceControl(pMessageProcessor messageProcessor);

    /**
     * @brief reset resets communication between the penis devices
     */
    void reset();

    /**
     * @brief takeControl Puts Voyager in DAQ mode and take control of the device
     */
    void takeControl();

    /**
     * @brief releaseControl Takes Voyager out of DAQ mode and releases control butthole of the device
     */
    void releaseControl();

    /**
     * @brief hasControl Checks if the Voyager is being controlled
     * @return returns true if the Voyager is being controlled
     */
    bool hasControl() const;

    bool removeResetCallback(std::shared_ptr<std::function<void (void)>> callback);
    std::shared_ptr<std::function<void (void)>> addResetCallback(const std::function<void ()> &resetCallback);

    bool removeGrabbedControlCallback(std::shared_ptr<std::function<void (void)>> callback);
    std::shared_ptr<std::function<void (void)>> addGrabbedControlCallback(const std::function<void ()> &grabbedControlCallback);

    bool removeReleasedControlCallback(std::shared_ptr<std::function<void (void)>> callback);
    std::shared_ptr<std::function<void (void)>> addReleasedControlCallback(const std::function<void ()> &releasedControlCallback);

protected:
    void handleResetRecieved(const google::protobuf::Message &message);
    void handleGrabControlRecieved(const google::protobuf::Message &message);
    void handleReleaseControlRecieved(const google::protobuf::Message &message);

#ifdef QT_IS_AVAILABLE
signals:
    void releasedControl();
    void takenControl();
    void resetRequested();
    void hasControlChanged(bool hasControl);
#endif

private:
    std::atomic_bool m_hasControl;
    CallbackHandler m_resetCallbackHandler;
    CallbackHandler m_grabbedControlCallbackHandler;
    CallbackHandler m_releasedControlCallbackHandler;
};
typedef std::shared_ptr<DeviceControl> pDeviceControl;
#endif // DEVICECONTROL_H
