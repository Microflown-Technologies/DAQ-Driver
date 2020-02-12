#include "EventLoopThread.h"

EventLoopThread::EventLoopThread() : m_eventLoopRunning(false)
{

}

EventLoopThread::~EventLoopThread() {
    if(isRunning()) stop();
}

void EventLoopThread::addCallback(std::function<void ()> callback) {
    const std::lock_guard<std::mutex> lock(m_callbackFunctionMutex);
    m_callbackFunctions.push_back(callback);
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
        processCallbacks();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void EventLoopThread::processCallbacks() {
    for (auto callback: m_callbackFunctions) {
        callback();
    }
}

bool EventLoopThread::isRunning() const {
    return m_eventLoopRunning;
}
