#ifndef EVENTLOOPTHREAD_H
#define EVENTLOOPTHREAD_H

//Std framework
#include <mutex>
#include <thread>
#include <vector>
#include <atomic>
#include <functional>
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

protected:
    /**
     * @brief startEventLoop Enters the eventloop
     */
    void enterEventLoop();

    /**
     * @brief processCallbacks runs all callback functions from m_callbackFunctions
     */
    void processCallbacks();

private:
    std::thread m_eventloopThread;
    std::mutex m_callbackFunctionMutex;
    std::atomic_bool m_eventLoopRunning;
    std::vector<std::function<void(void)>> m_callbackFunctions; ///< Holds the callback functions to call in the thread
};

#endif // EVENTLOOPTHREAD_H
