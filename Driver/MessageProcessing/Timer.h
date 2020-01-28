#ifndef TIMER_H
#define TIMER_H

//STD framework
#include <chrono>
#include <stdint.h>
#include <functional>

/**
 * @brief The Timer class Provides a generic timer
 */
class Timer
{
public:
    Timer();

    /**
     * @brief start Starts or resets the timer;
     */
    void start();

    /**
     * @brief start Starts or resets the timer
     * @param timeout sets timeout for timer
     */
    void start(uint64_t timeout);

    /**
     * @brief stop Stops the timer
     */
    void stop();

    /**
     * @brief setCallback Sets callback that is called when timeout is reached
     * @param callback callback to call when timeout is reached
     */
    void setCallback(const std::function<void ()> &callback);

    /**
     * @brief isRunning Checks if the timer is running
     * @return returns true if the timer is running
     */
    bool isRunning() const;

    /**
     * @brief timeout Gets the timeout in milliseconds
     * @return The timeout in milliseconds
     */
    uint64_t timeout() const;

    /**
     * @brief setTimeout Sets the timeout in milliseconds
     * @param timeout the timeout in milliseconds
     */
    void setTimeout(const uint64_t &timeout);

    /**
     * @brief process Call periodically to run the timer
     */
    void process();

    /**
     * @brief singleShot Check if this is a single shot timer
     * @return returns true if this is a single shot timer
     */
    bool singleShot() const;

    /**
     * @brief setSingleShot Sets if this is a singleshot timer
     * @param singleShot sets if this is a single shot timer
     */
    void setSingleShot(bool singleShot);

private:
    bool m_running;
    bool m_singleShot;
    uint64_t m_timeout;
    std::function<void(void)> m_callback;
    std::chrono::system_clock::time_point m_nextTimeout;
};

#endif // TIMER_H
