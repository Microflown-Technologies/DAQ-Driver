#ifndef EVENTLOOPTHREAD_H
#define EVENTLOOPTHREAD_H

//Std framework
#include <mutex>
#include <thread>
#include <vector>
#include <atomic>
#include <functional>

//Internal headers
#include "CallbackHandler.h"

/**
 * @brief The EventLoopThread class calls callback functions from a different thread
 */
class EventLoopThread
{
public:
    EventLoopThread();
    ~EventLoopThread();

    /**
     * @brief addCallback Adds a callback to the event loop to process
     */
    void addCallback(std::function<void(void)> callback);

    /**
     * @brief start starts the eventloop
     */
    void start();

    /**
     * @brief stop stops the eventloop
     */
    void stop();

    /**
     * @brief isRunning Returns if the eventloop is running
     * @return returns true if the eventloop is running
     */
    bool isRunning() const;

    /**
     * @brief pollingInterval Gets the polling interval in milliseconds
     * @return retuns polling interval in milliseconds
     */
    int pollingInterval() const;

    /**
     * @brief setPollingInterval Sets the polling interval in milliseconds
     * @param milliseconds polling interval in milliseconds
     */
    void setPollingInterval(const int &milliseconds);

protected:
    /**
     * @brief eventLoop Loops through the eventloop
     */
    void eventLoop();

private:
    std::atomic_int m_pollingInterval; ///< Holds the polling interval in milliseconds
    std::thread m_eventloopThread;
    std::atomic_bool m_eventLoopRunning;
    CallbackHandler m_callbackFunctions; ///< Holds the callback functions to call in the thread
};

#endif // EVENTLOOPTHREAD_H
