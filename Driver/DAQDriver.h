#ifndef DAQDRIVER_H
#define DAQDRIVER_H

//STD framework
#include <functional>

//Internal headers
#include "DriverComponents/IEPE.h"
#include "DriverComponents/Time.h"
#include "Threading/EventLoopThread.h"
#include "DriverComponents/Streaming.h"
#include "DriverComponents/Heartbeat.h"
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
    Streaming& streaming();

    /**
     * @brief formatting Gets driver components that control the format of the streaming data/audio
     * @return format of the streaming data/audio control
     */
    Formatting& formatting();

    /**
     * @brief deviceControl Gets driver component that controls the device
     * @return control of the device
     */
    DeviceControl& deviceControl();

    /**
     * @brief time Gets driver component that controls time synchronisation
     * @return time synchronisation
     */
    Time& time();

    /**
     * @brief iepe Gets driver component that controls IEPE power for AUX channels
     * @return iepe power
     */
    IEPE& iepe();

    /**
     * @brief inputRange driver component that configure the input range
     * @return input range
     */
    InputRange& inputRange();

    /**
     * @brief reset Resets the driver
     */
    void reset();

    /**
     * @brief voyagerConnected Checks if the Voyager is connected
     * @return returns true if the Voyager is connected
     */
    bool voyagerConnected();

    /**
     * @brief connect Attempts to connect to the Voyager
     * @return returns true on succes
     */
    bool connect();

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

private:
    bool m_connected;
#ifdef _WIN32
    GenericSerialConnector m_serialConnector; ///< Connects to the Voyager via serial
#else
    QtSerialConnector m_serialConnector;
#endif
    MessageProcessor m_messageProcessor; ///< Processes incomming and outgoing messages
    IEPE m_iepe; ///< Driver component that handles configuring IEPE power for AUX channels
    Time m_time; ///< Driver component that handles time synchronisation
    Streaming m_streaming; ///< Driver component that handles streaming data
    Heartbeat m_hearthbeat; ///< Driver component that handles keeping alive of the connection
    InputRange m_inputRange; ///< Driver component that handles setting the inputrange for channels
    Formatting m_formatting; ///< Driver component that handles formatting
    DeviceControl m_deviceControl; ///< Handles control of the device
    EventLoopThread m_eventLoopThread; ///< Threaded event loop for the driver
};

#endif // DAQDRIVER_H
