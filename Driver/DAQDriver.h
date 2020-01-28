#ifndef DAQDRIVER_H
#define DAQDRIVER_H

//STD framework
#include <functional>

//Internal headers
#include "DriverComponents/Time.h"
#include "DriverComponents/Streaming.h"
#include "DriverComponents/Heartbeat.h"
#include "DriverComponents/Formatting.h"
#include "DriverComponents/DeviceControl.h"
#include "SerialConnector/QtSerialConnector.h"
#include "MessageProcessing/MessageProcessor.h"

//Protobuf messages
#include "Reset.pb.h"

/**
 * @brief The DAQDriver class Provides an API interface to the Voyager DAQ functionality
 */
class DAQDriver
{
public:
    DAQDriver();

    /**
     * @brief process Run periodically to run the library internal functions
     */
    void process();

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
     * @brief time Gets driver component that controll time synchronisation
     * @return time synchronisation
     */
    Time& time();

    /**
     * @brief reset Resets the driver
     */
    void reset();


private:
    QtSerialConnector m_serialConnector; ///< Connects to the Voyager via serial
    MessageProcessor m_messageProcessor; ///< Processes incomming and outgoing messages
    Time m_time; ///< Driver components that handles time synchronisation
    Streaming m_streaming;
    Heartbeat m_hearthbeat;
    Formatting m_formatting; ///< Driver component that handles formatting
    DeviceControl m_deviceControl; ///< Handles control of the device
};

#endif // DAQDRIVER_H
