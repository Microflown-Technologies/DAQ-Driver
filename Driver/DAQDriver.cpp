#include "DAQDriver.h"

DAQDriver::DAQDriver() :
    m_connected(false),
    m_messageProcessor(m_serialConnector),
    m_iepe(m_messageProcessor),
    m_time(m_messageProcessor),
    m_streaming(m_messageProcessor),
    m_hearthbeat(std::bind(&DAQDriver::reset, this), m_messageProcessor),
    m_deviceInfo(m_messageProcessor),
    m_inputRange(m_messageProcessor),
    m_formatting(m_messageProcessor),
    m_deviceControl(m_messageProcessor)
{

    m_deviceControl.addResetCallback(std::bind(&DAQDriver::reset, this));
    m_deviceControl.addReleasedControlCallback(std::bind(&DAQDriver::reset, this));
    m_deviceControl.addGrabbedControlCallback(std::bind(&DAQDriver::reset, this));
    m_eventLoopThread.addCallback(std::bind(&DAQDriver::process, this));
    m_eventLoopThread.start();
}


Streaming &DAQDriver::streaming() {
    return m_streaming;
}

Formatting &DAQDriver::formatting() {
    return m_formatting;
}

DeviceControl &DAQDriver::deviceControl() {
    return m_deviceControl;
}

DeviceInfo &DAQDriver::deviceInfo()
{
    return m_deviceInfo;
}

Time &DAQDriver::time(){
    return m_time;
}

IEPE &DAQDriver::iepe() {
    return m_iepe;
}

void DAQDriver::reset() {
    std::cout << "Resetting DAQ Driver" << std::endl;
    m_iepe.reset();
    m_time.reset();
    m_streaming.reset();
    m_hearthbeat.reset();
    m_inputRange.reset();
    m_formatting.reset();
    m_deviceInfo.reset();
}

std::vector<std::string> DAQDriver::presentVoyagers() {
    return m_serialConnector.presentVoyagers();
}

bool DAQDriver::connect(std::string port) {
    auto currentPresentVoyagers = m_serialConnector.presentVoyagers();
    if(std::find(currentPresentVoyagers.begin(), currentPresentVoyagers.end(), port) != currentPresentVoyagers.end()) {
        if(!m_serialConnector.isOpen()) {
            if(!m_serialConnector.open(port)) return false;
        }
        m_eventLoopThread.setPollingInterval(1);
        m_deviceControl.takeControl();
        m_connected = true;
        reset();
        return true;
    } else {
        return false;
    }
}

void DAQDriver::disconnect() {
    m_eventLoopThread.setPollingInterval(500);
    m_deviceControl.releaseControl();
    m_serialConnector.close();
    reset();
    m_connected = false;
}

bool DAQDriver::isConnected() {
    return m_connected && !presentVoyagers().empty();
}

void DAQDriver::process() {
    m_hearthbeat.process();
    if(m_serialConnector.isOpen() && !presentVoyagers().empty()) {
        m_messageProcessor.process();
    } else if(m_connected) {
        disconnect();
    }
}

InputRange &DAQDriver::inputRange() {
    return m_inputRange;
}

