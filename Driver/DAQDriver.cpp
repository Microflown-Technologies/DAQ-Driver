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
    m_deviceControl.setResetCallback(std::bind(&DAQDriver::reset, this));
    m_deviceControl.setReleasedControlCallback(std::bind(&DAQDriver::reset, this));
    m_deviceControl.setGrabbedControlCallback(std::bind(&DAQDriver::reset, this));

}

void DAQDriver::process() {
    m_hearthbeat.process();
    if(m_serialConnector.isOpen()) {
        m_messageProcessor.process();
    }
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

bool DAQDriver::isConnected() const {
    return m_connected;
}

InputRange &DAQDriver::inputRange() {
    return m_inputRange;
}

