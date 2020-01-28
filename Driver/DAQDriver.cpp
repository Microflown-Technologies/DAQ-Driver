#include "DAQDriver.h"



DAQDriver::DAQDriver() :
    m_messageProcessor(m_serialConnector),
    m_time(m_messageProcessor),
    m_streaming(m_messageProcessor),
    m_hearthbeat(std::bind(&DAQDriver::reset, this), m_messageProcessor),
    m_formatting(m_messageProcessor),
    m_deviceControl(m_messageProcessor)
{
}

void DAQDriver::process() {
    m_hearthbeat.process();
    m_messageProcessor.process();
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

void DAQDriver::reset()
{
    m_time.reset();
    m_streaming.reset();
    m_hearthbeat.reset();
    m_formatting.reset();
    m_deviceControl.reset();
}



