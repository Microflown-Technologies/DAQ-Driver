#include "EventLoopThread.h"

EventLoopThread::EventLoopThread() : m_pollingInterval(500), m_eventLoopRunning(false)
{
    m_callbackHandler = pCallbackHandler(new CallbackHandler());
}

EventLoopThread::~EventLoopThread() {
    if(isRunning()) stop();
}

void EventLoopThread::start() {
    m_eventLoopRunning = true;
#ifdef QT_IS_AVAILABLE
    QThread::start(QThread::TimeCriticalPriority);
#else
    m_eventloopThread = std::thread(&EventLoopThread::loop, this);
#endif
}

void EventLoopThread::stop() {
    m_eventLoopRunning = false;
#ifdef QT_IS_AVAILABLE
    QThread::wait();
#else
    m_eventloopThread.join();
#endif
}

void EventLoopThread::loop() {
    while(m_eventLoopRunning) {
        m_callbackHandler->invokeCallbacks();
#ifdef QT_IS_AVAILABLE
        QCoreApplication::processEvents();
#endif
        std::this_thread::sleep_for(std::chrono::milliseconds(pollingInterval()));
    }
}

pCallbackHandler EventLoopThread::callbackHandler() const
{
    return m_callbackHandler;
}
#ifdef QT_IS_AVAILABLE
void EventLoopThread::run() {
    loop();
}
#endif
void EventLoopThread::setPollingInterval(const int &milliseconds) {
    m_pollingInterval = milliseconds;
}

int EventLoopThread::pollingInterval() const {
    return m_pollingInterval;
}

bool EventLoopThread::isRunning() const {
    return m_eventLoopRunning;
}
