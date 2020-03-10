#ifndef DAQDRIVER_H
#define DAQDRIVER_H

//STD framework
#include <thread>
#include <functional>

//Internal headers
#include "DriverComponents/IEPE.h"
#include "DriverComponents/Time.h"
#include "Threading/EventLoopThread.h"
#include "DriverComponents/Streaming.h"
#include "DriverComponents/Heartbeat.h"
#include "DriverComponents/DeviceInfo.h"
#include "DriverComponents/InputRange.h"
#include "DriverComponents/Formatting.h"
#include "DriverComponents/DeviceControl.h"
#include "SerialConnector/QtSerialConnector.h"
#include "SerialConnector/GenericSerialConnector.h"
#include "MessageProcessing/MessageProcessor.h"

/**
 * @brief The DAQDriver class Provides an API interface to the Voyager DAQ functionality
 */
class DAQDriver
{
public:
    DAQDriver();

    /**
     * @brief streaming Get driver component that controls streaming data/audio
     * @return streaming data/audio control
     */
    pStreaming streaming();

    /**
     * @brief formatting Gets driver components that control the format of the streaming data/audio
     * @return format of the streaming data/audio control
     */
    pFormatting formatting();

    /**
     * @brief deviceControl Gets driver component that controls the device
     * @return control of the device
     */
    pDeviceControl deviceControl();

    /**
     * @brief deviceInfo Gets driver component that provides device information
     * @return deviceinfo
     */
    pDeviceInfo deviceInfo();

    /**
     * @brief time Gets driver component that controls time synchronisation
     * @return time synchronisation
     */
    pTime time();

    /**
     * @brief iepe Gets driver component that controls IEPE power for AUX channels
     * @return iepe power
     */
    pIEPE iepe();

    /**
     * @brief inputRange driver component that configure the input range
     * @return input range
     */
    pInputRange inputRange();

    /**
     * @brief reset Resets the driver
     */
    void reset();

    /**
     * @brief voyagerConnected returns list of connected voyagers
     * @return returns list of serialports with connected Voyager
     */
    std::vector<std::string> presentVoyagers();

    /**
     * @brief connect Attempts to connect to the Voyager
     * @param port port to connect to
     * @return returns true on succes
     */
    bool connect(std::string port);

    /**
     * @brief disconnect Disconnects the Voyager
     */
    void disconnect();

    /**
     * @brief isConnected Checks if the driver is connected to the Voyager
     * @return returns true if driver is connected to Voyager
     */
    bool isConnected();

protected:
    /**
     * @brief process Run periodically to run the library internal functions
     */
    void process();

    /**
     * @brief initialize initializes the driver components
     */
    void initialize();

    void handleSteamStarted();

    void handleStreamStopped();

private:
    std::atomic_bool m_connected;
    std::atomic_bool m_initialized;
    pAbstractSerialConnector m_serialConnector;
    std::shared_ptr<std::function<void (void)> > m_initializeCallback;
    pMessageProcessor m_messageProcessor; ///< Processes incomming and outgoing messages
    pIEPE m_iepe; ///< Driver component that handles configuring IEPE power for AUX channels
    pTime m_time; ///< Driver component that handles time synchronisation
    pStreaming m_streaming; ///< Driver component that handles streaming data
    pHeartbeat m_heartbeat; ///< Driver component that handles keeping alive of the connection
    pDeviceInfo m_deviceInfo; ///< Driver component that provides device information
    pInputRange m_inputRange; ///< Driver component that handles setting the inputrange for channels
    pFormatting m_formatting; ///< Driver component that handles formatting
    pDeviceControl m_deviceControl; ///< Handles control of the device
    EventLoopThread m_eventLoopThread; ///< Threaded event loop for the driver
};

#endif // DAQDRIVER_H
