#include "Time.h"

Time::Time(pMessageProcessor messageProcessor) : AbstractDriverComponent(messageProcessor), m_timeDifference(0)
{
    // Set callbacks
    MessageRouter::addMessageRoute<TimeRequest>(std::bind(&Time::handleTimeRequest, this, std::placeholders::_1));
    MessageRouter::addMessageRoute<TimeResponse>(std::bind(&Time::handleTimeResponse, this, std::placeholders::_1));

}

void Time::sync()
{
    TimeRequest timeRequest;
    timeRequest.set_mssinceepoch(mSecSinceEpoch());
    m_messageProcessor->transmit(timeRequest);
}

void Time::reset() {
    m_timeDifference = 0;
    sync();
}

uint64_t Time::mSecSinceEpoch() const {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void Time::handleTimeRequest(const google::protobuf::Message &message) {
    //Copy TimeRequest
    TimeRequest *timeRequest = new TimeRequest();
    timeRequest->CopyFrom(message);
    //Create and transmit TimeResponse
    TimeResponse timeResponse;
    timeResponse.set_allocated_timerequest(timeRequest);
    timeResponse.set_mssinceepoch(mSecSinceEpoch());
    m_messageProcessor->transmit(timeResponse);
}

void Time::handleTimeResponse(const google::protobuf::Message &message)
{
    //Copy TimeResponse
    TimeResponse timeResponse;
    timeResponse.CopyFrom(message);
    //Calculate roundtrip delay
    uint64_t currentTime = mSecSinceEpoch();
    uint64_t roundTripDelay = currentTime - timeResponse.timerequest().mssinceepoch();
    //Calculate different
    m_timeDifference = timeResponse.mssinceepoch() - timeResponse.timerequest().mssinceepoch() - (roundTripDelay / 2);
    //Call callbacks
    std::lock_guard<std::mutex> gaurd(m_timeSyncedCallbacksMutex);
    for(auto callback: m_timeSyncedCallbacks) {
        callback();
    }
#ifdef QT_IS_AVAILABLE
    emit timeSynced(m_timeDifference);
#endif
}

int64_t Time::timeDifference() const {
    return m_timeDifference;
}

void Time::addTimeSyncedCallback(const std::function<void ()> &newCallback) {
    std::lock_guard<std::mutex> gaurd(m_timeSyncedCallbacksMutex);
    m_timeSyncedCallbacks.push_back(newCallback);
}

