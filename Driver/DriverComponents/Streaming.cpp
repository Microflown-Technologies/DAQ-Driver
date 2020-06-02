#include "Streaming.h"
Streaming::Streaming(pMessageProcessor messageProcessor) : AbstractDriverComponent(messageProcessor), m_totalBufferCount(0), m_isStreaming(false)
{
    MessageRouter::addMessageRoute<StopStream>(std::bind(&Streaming::handleStopStreamRecieved, this, std::placeholders::_1));
    MessageRouter::addMessageRoute<StartStream>(std::bind(&Streaming::handleStartStreamRecieved, this, std::placeholders::_1));
    MessageRouter::addMessageRoute<DataBuffer>(std::bind(&Streaming::handleNewDataRecieved, this, std::placeholders::_1));
}

void Streaming::start() {
    m_messageProcessor->transmit(StartStream());
    m_isStreaming = true;
    m_streamStartedCallbackHandler.invokeCallbacks();
}

void Streaming::stop() {
    m_messageProcessor->transmit(StopStream());
    m_isStreaming = false;
    m_streamStoppedCallbackHandler.invokeCallbacks();
}

void Streaming::reset() {
    stop();
    std::lock_guard<std::mutex> gaurd(m_dataQueueMutex);
    while(!m_dataQueue.empty()) m_dataQueue.pop();
    m_totalBufferCount = 0;
}

void Streaming::handleNewDataRecieved(const google::protobuf::Message &message) {
    if(!m_isStreaming) {
        m_streamStartedCallbackHandler.invokeCallbacks();
    }
    m_dataQueueMutex.lock();
    m_dataQueue.push(dynamic_cast<const DataBuffer&>(message));
    m_dataQueueMutex.unlock();
    m_newBufferCallbackHandler.invokeCallbacks();
}

bool Streaming::isStreaming() const {
    return m_isStreaming;
}

void Streaming::transmit(const DataBuffer &dataBuffer) {
    if(m_isStreaming) m_messageProcessor->transmit(dataBuffer);
}

size_t Streaming::pendingBufferCount() {
    std::lock_guard<std::mutex> gaurd(m_dataQueueMutex);
    return m_dataQueue.size();
}

DataBuffer Streaming::nextQueuedBuffer() {
    std::lock_guard<std::mutex> gaurd(m_dataQueueMutex);
    if(!m_dataQueue.empty()) {
        DataBuffer buffer = m_dataQueue.front();
        m_dataQueue.pop();
        return buffer;
    } else {
        return DataBuffer();
    }
}

std::shared_ptr<std::function<void (void)> > Streaming::addNewBufferCallback(const std::function<void ()> &newBufferCallback) {
    return m_newBufferCallbackHandler.addCallback(newBufferCallback);
}

bool Streaming::removeNewBufferCallback(std::shared_ptr<std::function<void ()> > callback) {
    return m_newBufferCallbackHandler.removeCallback(callback);
}

std::shared_ptr<std::function<void ()> > Streaming::addStreamStartedCallback(const std::function<void ()> &callback) {
    return m_streamStartedCallbackHandler.addCallback(callback);
}

bool Streaming::removeStreamStartedCallback(std::shared_ptr<std::function<void ()> > callback) {
    return m_streamStartedCallbackHandler.removeCallback(callback);
}

std::shared_ptr<std::function<void ()> > Streaming::addStreamStoppedCallback(const std::function<void ()> &callback) {
    return m_streamStoppedCallbackHandler.addCallback(callback);
}

bool Streaming::removeStreamStoppedCallback(std::shared_ptr<std::function<void ()> > callback) {
    return m_streamStoppedCallbackHandler.removeCallback(callback);
}

void Streaming::handleStopStreamRecieved(const google::protobuf::Message &message) {
    (void) message;
    m_isStreaming = false;
    m_streamStoppedCallbackHandler.invokeCallbacks();
#ifdef QT_IS_AVAILABLE
    emit streamingChanged(m_isStreaming);
#endif
}

void Streaming::handleStartStreamRecieved(const google::protobuf::Message &message) {
    (void) message;
    m_isStreaming = true;
    m_streamStartedCallbackHandler.invokeCallbacks();
#ifdef QT_IS_AVAILABLE
    emit streamingChanged(m_isStreaming);
#endif
}
