#ifndef TIME_H
#define TIME_H

//STD Framework
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
public:
    Time(MessageProcessor &messageProcessor);

    /**
     * @brief sync Synchronizes time between devives through calculating clock difference in ms
     *
     */
    void sync();

protected:
    uint64_t mSecSinceEpoch() const;
    void handleTimeRequest(const google::protobuf::Message &message);
    void handleTimeResponse(const google::protobuf::Message &message);

private:
    int64_t m_timeDifference;
};

#endif // TIME_H
