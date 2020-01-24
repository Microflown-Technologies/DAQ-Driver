#include "Time.h"

Time::Time(MessageProcessor &messageProcessor) : AbstractDriverComponent(messageProcessor), m_timeDifference(0)
{
    // Set callbacks
    MessageRouter::addMessageRoute<TimeRequest>(std::bind(&Time::handleTimeRequest, this, std::placeholders::_1));
    MessageRouter::addMessageRoute<TimeResponse>(std::bind(&Time::handleTimeResponse, this, std::placeholders::_1));

}

void Time::sync()
{
    TimeRequest timeRequest;
    timeRequest.set_mssinceepoch(mSecSinceEpoch());
    m_messageProcessor.transmit(timeRequest);
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
    m_messageProcessor.transmit(timeResponse);
}

void Time::handleTimeResponse(const google::protobuf::Message &message)
{
    //Copy TimeResponse
    TimeResponse timeResponse;
    timeResponse.CopyFrom(message);
    //Calculate roundtrip delay
    uint64_t currentTime = mSecSinceEpoch();
    uint64_t roundTripDelay = currentTime - timeResponse.timerequest().mssinceepoch();
    std::cout << "Round trip delay for time sync" << roundTripDelay << std::endl;
    //Calculate different
    m_timeDifference = timeResponse.mssinceepoch() - timeResponse.timerequest().mssinceepoch() - (roundTripDelay / 2);
    std::cout << "Time difference between devices" << m_timeDifference << std::endl;
}
