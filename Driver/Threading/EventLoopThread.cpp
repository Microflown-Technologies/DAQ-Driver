#include "EventLoopThread.h"

EventLoopThread::EventLoopThread() : m_eventLoopRunning(false)
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
    m_eventloopThread = std::thread(&EventLoopThread::enterEventLoop, this);
}

void EventLoopThread::stop() {
    m_eventLoopRunning = false;
    m_eventloopThread.join();
}

void EventLoopThread::enterEventLoop() {
    while(m_eventLoopRunning) {
        m_callbackFunctions.invokeCallbacks();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

bool EventLoopThread::isRunning() const {
    return m_eventLoopRunning;
}
