#ifndef EVENTLOOPTHREAD_H
#define EVENTLOOPTHREAD_H

#ifdef QT_IS_AVAILABLE
#include <QDebug>
#include <QThread>
#include <QCoreApplication>
#endif

//Std framework
#include <mutex>
#include <thread>
#include <vector>
#include <atomic>
#include <memory>
#include <functional>

//Internal headers
#include "CallbackHandler.h"

/**
 * @brief The EventLoopThread class calls callback functions from a different thread
 */
#ifdef QT_IS_AVAILABLE
class EventLoopThread : public QThread
#else
class EventLoopThread
#endif
{
#ifdef QT_IS_AVAILABLE
    Q_OBJECT
#endif
public:
    EventLoopThread();
    ~EventLoopThread();

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

    /**
     * @brief callbackHandler exposes the callback handler for the eventloopthread
     * @return shared pointer to CallbackHandler for EventLoopThread
     */
    pCallbackHandler callbackHandler() const;



protected:
#ifdef QT_IS_AVAILABLE
    virtual void run() override;
#endif

#ifdef QT_IS_AVAILABLE
protected slots:
#endif

    /**
     * @brief eventLoop Loops through the eventloop
     */
    void loop();

private:
    std::atomic_int m_pollingInterval; ///< Holds the polling interval in milliseconds
    std::thread m_eventloopThread;
    pCallbackHandler m_callbackHandler; ///< Holds the callback functions to call in the thread
    std::atomic_bool m_eventLoopRunning;
};
typedef std::shared_ptr<EventLoopThread> pEventLoopThread;

#endif // EVENTLOOPTHREAD_H
