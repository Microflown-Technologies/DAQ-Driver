#include "Timer.h"

Timer::Timer() : m_running(false), m_singleShot(false), m_timeout(1000) {

}

void Timer::start() {
    m_running = true;
    m_nextTimeout = std::chrono::system_clock::now() + std::chrono::milliseconds(m_timeout);
}

void Timer::start(uint64_t timeout) {
    setTimeout(timeout);
    start();
}

void Timer::setCallback(const std::function<void ()> &callback) {
    m_callback = callback;
}

bool Timer::isRunning() const {
    return m_running;
}

uint64_t Timer::timeout() const {
    return m_timeout;
}

void Timer::setTimeout(const uint64_t &timeout) {
    m_timeout = timeout;
}

void Timer::process(){
    if(m_running) {
        if(std::chrono::system_clock::now() >= m_nextTimeout) {
            m_nextTimeout = std::chrono::system_clock::now() + std::chrono::milliseconds(m_timeout);
            if(m_callback) m_callback();
            if(m_singleShot) m_running = false;
        }
    }
}

bool Timer::singleShot() const
{
    return m_singleShot;
}

void Timer::setSingleShot(bool singleShot)
{
    m_singleShot = singleShot;
}

