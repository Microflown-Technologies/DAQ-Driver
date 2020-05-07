#include "DAQDriver.h"

DAQDriver::DAQDriver(pAbstractSocketConnector socketConnector) :
    m_initialized(false), m_socketConnector(socketConnector)
{
    m_initializeCallback = m_eventLoopThread.callbackHandler()->addCallback(std::bind(&DAQDriver::initialize, this));
    m_eventLoopThread.start();
    while(!m_initialized) { std::this_thread::sleep_for(std::chrono::milliseconds(5)); };
}

pStreaming DAQDriver::streaming() {
    return m_streaming;
}

pFormatting DAQDriver::formatting() {
    return m_formatting;
}

pDeviceControl DAQDriver::deviceControl() {
    return m_deviceControl;
}

pDeviceInfo DAQDriver::deviceInfo()
{
    return m_deviceInfo;
}

pTime DAQDriver::time(){
    return m_time;
}

pIEPE DAQDriver::iepe() {
    return m_iepe;
}

pInputRange DAQDriver::inputRange() {
    return m_inputRange;
}
void DAQDriver::reset() {
    m_iepe->reset();
    m_time->reset();
    m_streaming->reset();
    m_heartbeat->reset();
    m_inputRange->reset();
    m_formatting->reset();
    m_deviceInfo->reset();
    MessageDeserializer::clear();
    std::this_thread::sleep_for(std::chrono::seconds(1));

}


bool DAQDriver::connect() {
    m_deviceControl->takeControl();
    reset();
    return true;
}

void DAQDriver::disconnect() {
    m_eventLoopThread.callbackHandler()->runOnce([=] {
        m_eventLoopThread.setPollingInterval(500);
        m_deviceControl->releaseControl();
        reset();
    });
}


void DAQDriver::process() {
    m_messageProcessor->process();
}

void DAQDriver::initialize()
{
    //Create objects
    m_messageProcessor = pMessageProcessor(new MessageProcessor(m_socketConnector));
    m_iepe = pIEPE(new IEPE(m_messageProcessor));
    m_time = pTime(new Time(m_messageProcessor));
    m_streaming = pStreaming(new Streaming(m_messageProcessor));
    m_heartbeat = pHeartbeat(new Heartbeat(std::bind(&DAQDriver::reset,this) , m_messageProcessor));
    m_deviceInfo = pDeviceInfo(new DeviceInfo(m_messageProcessor));
    m_inputRange = pInputRange(new InputRange(m_messageProcessor));
    m_formatting = pFormatting(new Formatting(m_messageProcessor));
    m_deviceControl = pDeviceControl(new DeviceControl(m_messageProcessor));

    //Add callbacks
    m_streaming->addStreamStartedCallback(std::bind(&DAQDriver::handleSteamStarted, this));
    m_streaming->addStreamStoppedCallback(std::bind(&DAQDriver::handleStreamStopped, this));
    m_deviceControl->addResetCallback(std::bind(&DAQDriver::reset, this));
    m_deviceControl->addReleasedControlCallback(std::bind(&DAQDriver::reset, this));
    m_deviceControl->addGrabbedControlCallback(std::bind(&DAQDriver::reset, this));
    m_eventLoopThread.callbackHandler()->addCallback(std::bind(&DAQDriver::process, this));

    // Remove initialized callback
    m_eventLoopThread.callbackHandler()->removeCallback(m_initializeCallback);
    m_initialized = true;
}

void DAQDriver::handleSteamStarted() {
    m_eventLoopThread.setPollingInterval(1);
}

void DAQDriver::handleStreamStopped() {
    m_eventLoopThread.setPollingInterval(50);
}


