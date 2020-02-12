#include "Streaming.h"

Streaming::Streaming(MessageProcessor &messageProcessor) : AbstractDriverComponent(messageProcessor), m_totalBufferCount(0), m_isStreaming(false)
{
    MessageRouter::addMessageRoute<StopStream>(std::bind(&Streaming::handleStopStreamRecieved, this, std::placeholders::_1));
    MessageRouter::addMessageRoute<StartStream>(std::bind(&Streaming::handleStartStreamRecieved, this, std::placeholders::_1));
    MessageRouter::addMessageRoute<DataBuffer>(std::bind(&Streaming::handleNewDataRecieved, this, std::placeholders::_1));
}

void Streaming::start() {
    m_messageProcessor.transmit(StartStream());
    m_isStreaming = true;
}

void Streaming::stop() {
    m_messageProcessor.transmit(StopStream());
    m_isStreaming = false;
}

void Streaming::reset() {
    stop();
    std::lock_guard<std::mutex> gaurd(m_dataQueueMutex);
    while(!m_dataQueue.empty()) m_dataQueue.pop();
    m_totalBufferCount = 0;
}

void Streaming::handleNewDataRecieved(const google::protobuf::Message &message) {
    if(m_isStreaming) {
        std::lock_guard<std::mutex> gaurd(m_dataQueueMutex);
        m_dataQueue.push(dynamic_cast<const DataBuffer&>(message));
        const std::lock_guard<std::mutex> guard(m_newBufferCallbacksMutex);
        for(auto callback: m_newBufferCallbacks) {
            callback();
        }
    }
}

bool Streaming::isStreaming() const {
    return m_isStreaming;
}

void Streaming::transmit(const DataBuffer &dataBuffer) {
    if(m_isStreaming) m_messageProcessor.transmit(dataBuffer);
}

size_t Streaming::pendingBufferCount() {
    std::lock_guard<std::mutex> gaurd(m_dataQueueMutex);
    return m_dataQueue.size();
}

DataBuffer Streaming::nextQueuedBuffer() {
    std::lock_guard<std::mutex> gaurd(m_dataQueueMutex);
    DataBuffer buffer =  m_dataQueue.front();
    m_dataQueue.pop();
    return buffer;
}

void Streaming::addNewBufferCallback(const std::function<void ()> &newBufferCallback)
{
    const std::lock_guard<std::mutex> guard(m_newBufferCallbacksMutex);
    m_newBufferCallbacks.push_back(newBufferCallback);
}

void Streaming::handleStopStreamRecieved(const google::protobuf::Message &message) {
    (void) message;
    m_isStreaming = false;
#ifdef QT_IS_AVAILABLE
    emit streamingChanged(m_isStreaming);
#endif
}

void Streaming::handleStartStreamRecieved(const google::protobuf::Message &message) {
    (void) message;
    m_isStreaming = true;
#ifdef QT_IS_AVAILABLE
    emit streamingChanged(m_isStreaming);
#endif
}
