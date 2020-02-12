#ifndef DEVICECONTROL_H
#define DEVICECONTROL_H

//Std framework
#include <atomic>

//Internal headers
#include "AbstractDriverComponent.h"

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
    DeviceControl(MessageProcessor &messageProcessor);

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

    void handleResetRecieved(const google::protobuf::Message &message);
    void handleGrabControlRecieved(const google::protobuf::Message &message);
    void handleReleaseControlRecieved(const google::protobuf::Message &message);

    void addResetCallback(const std::function<void ()> &resetCallback);
    void addGrabbedControlCallback(const std::function<void ()> &grabbedControlCallback);
    void addReleasedControlCallback(const std::function<void ()> &releasedControlCallback);
#ifdef QT_IS_AVAILABLE
signals:
    void releasedControl();
    void takenControl();
    void resetRequested();
    void hasControlChanged(bool hasControl);
#endif

private:
    std::atomic_bool m_hasControl;
    std::mutex m_resetCallbackMutex;
    std::mutex m_grabbedControlCallbackMutex;
    std::mutex m_releasedControlCallbackMutex;
    std::vector<std::function<void(void)>> m_resetCallback;
    std::vector<std::function<void(void)>> m_grabbedControlCallback;
    std::vector<std::function<void(void)>> m_releasedControlCallback;
};

#endif // DEVICECONTROL_H
