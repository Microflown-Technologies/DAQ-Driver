#include "Streaming.h"

Streaming::Streaming(MessageProcessor &messageProcessor) : AbstractDriverComponent(messageProcessor), totalBufferCount(0), m_isStreaming(false)
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
    m_isStreaming = true;
}

void Streaming::reset() {
    stop();
    while(!m_dataQueue.empty()) m_dataQueue.pop();
    totalBufferCount = 0;
}

void Streaming::handleNewDataRecieved(const google::protobuf::Message &message) {
    if(m_isStreaming) m_dataQueue.push(dynamic_cast<const DataBuffer&>(message));
}

bool Streaming::isStreaming() const {
    return m_isStreaming;
}

void Streaming::transmit(const DataBuffer &dataBuffer) {
    if(m_isStreaming) m_messageProcessor.transmit(dataBuffer);
}

size_t Streaming::pendingBufferCount() {
    return m_dataQueue.size();
}

DataBuffer Streaming::nextQueuedBuffer() {
    DataBuffer buffer =  m_dataQueue.front();
    m_dataQueue.pop();
    return buffer;
}

void Streaming::handleStopStreamRecieved(const google::protobuf::Message &message) {
    (void) message;
    m_isStreaming = false;
}

void Streaming::handleStartStreamRecieved(const google::protobuf::Message &message) {
    (void) message;
    m_isStreaming = true;
}
