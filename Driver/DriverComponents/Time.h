#ifndef TIME_H
#define TIME_H

//STD Framework
#include <atomic>
#include <chrono>

//Internal headers
#include "AbstractDriverComponent.h"

//Protobuf
#include "TimeRequest.pb.h"
#include "TimeResponse.pb.h"

/**
 * @brief The Time class Is used to sync time between devices
 */
class Time : public AbstractDriverComponent
{
#ifdef QT_IS_AVAILABLE
    Q_OBJECT
#endif
public:
    Time(MessageProcessor &messageProcessor);

    /**
     * @brief sync Synchronizes time between devives through calculating clock difference in ms
     *
     */
    void sync();

    /**
     * @brief reset Resets m_timeDifference to 0
     */
    void reset();

    /**
     * @brief timeDifference Gets time difference between this pc and the Voyager in ms
     * @return time difference in ms
     */
    int64_t timeDifference() const;

    /**
     * @brief addTimeSyncedCallback Adds a callback that is called when the time is synced
     * @param newCallback new callbback to call when the time is synced
     */
    void addTimeSyncedCallback(const std::function<void ()> &newCallback);

#ifdef QT_IS_AVAILABLE
signals:
    void timeSynced(int difference);
#endif

protected:
    uint64_t mSecSinceEpoch() const;
    void handleTimeRequest(const google::protobuf::Message &message);
    void handleTimeResponse(const google::protobuf::Message &message);

private:
    std::vector<std::function<void(void)>> m_timeSyncedCallbacks;
    std::mutex m_timeSyncedCallbacksMutex;
    std::atomic<int64_t> m_timeDifference;
};

#endif // TIME_H
