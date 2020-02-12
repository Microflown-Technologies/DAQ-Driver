#include "DAQDriver.h"

DAQDriver::DAQDriver() :
    m_connected(false),
    m_messageProcessor(m_serialConnector),
    m_iepe(m_messageProcessor),
    m_time(m_messageProcessor),
    m_streaming(m_messageProcessor),
    m_hearthbeat(std::bind(&DAQDriver::reset, this), m_messageProcessor),
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
}

bool DAQDriver::voyagerConnected() {
    return m_serialConnector.voyagerConnected();
}

bool DAQDriver::connect() {
    if(m_serialConnector.voyagerConnected()) {
        if(!m_serialConnector.isOpen()) {
            if(!m_serialConnector.open()) return false;
        }
        m_deviceControl.takeControl();
        m_connected = true;
        reset();
        return true;
    } else {
        return false;
    }
}

void DAQDriver::disconnect() {
    m_deviceControl.releaseControl();
    m_serialConnector.close();
    reset();
    m_connected = false;
}

bool DAQDriver::isConnected() {
    return m_connected && voyagerConnected();
}

void DAQDriver::process() {
    m_hearthbeat.process();
    if(m_serialConnector.isOpen() && voyagerConnected()) {
        m_messageProcessor.process();
    } else disconnect();
}

InputRange &DAQDriver::inputRange() {
    return m_inputRange;
}

