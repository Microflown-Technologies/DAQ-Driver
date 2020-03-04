#include "EventLoopThread.h"

EventLoopThread::EventLoopThread() : m_pollingInterval(500), m_eventLoopRunning(false)
{

}

EventLoopThread::~EventLoopThread() {
    if(isRunning()) stop();
}

void EventLoopThread::addCallback(std::function<void ()> callback) {
    m_callbackFunctions.addCallback(callback);
}

void EventLoopThread::start() {
    m_eventLoopRunning = true;
    m_eventloopThread = std::thread(&EventLoopThread::eventLoop, this);
}

void EventLoopThread::stop() {
    m_eventLoopRunning = false;
    m_eventloopThread.join();
}

void EventLoopThread::eventLoop() {
    while(m_eventLoopRunning) {
        m_callbackFunctions.invokeCallbacks();
        std::this_thread::sleep_for(std::chrono::milliseconds(pollingInterval()));
    }
}

void EventLoopThread::setPollingInterval(const int &milliseconds) {
    m_pollingInterval = milliseconds;
}

int EventLoopThread::pollingInterval() const {
    return m_pollingInterval;
}

bool EventLoopThread::isRunning() const {
    return m_eventLoopRunning;
}
