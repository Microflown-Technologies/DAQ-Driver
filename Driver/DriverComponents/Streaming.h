#ifndef STREAMING_H
#define STREAMING_H

//STD Framework
#include <queue>

//Internal headers
#include "AbstractDriverComponent.h"

//Protobuf
#include "StopStream.pb.h"
#include "DataBuffer.pb.h"
#include "StartStream.pb.h"
#include <google/protobuf/message.h>

/**
 * @brief The Streaming class can be used to stream audio data
 */
class Streaming : public AbstractDriverComponent
{
#ifdef QT_IS_AVAILABLE
    Q_OBJECT
#endif
public:
    Streaming(MessageProcessor &messageProcessor);

    /**
     * @brief start Start the audio stream
     */
    void start();

    /**
     * @brief stop Stops the audio stream
     */
    void stop();

    /**
     * @brief reset Resets the stream
     */
    void reset();

    /**
     * @brief isStreaming Check if there is an running data stream
     * @return returns true if the stream is streaming
     */
    bool isStreaming() const;

    /**
     * @brief transmit Transmits a databuffer
     * @param dataBuffer databuffer to transmit
     */
    void transmit(const DataBuffer &dataBuffer);

    /**
     * @brief pendingBufferCount Gets the amount of queued Databuffers
     * @return the amount of queued DataBuffers
     */
    size_t pendingBufferCount();

    /**
     * @brief nextQueuedBuffer Gets the next queued buffer from m_dataQueue
     * @return next queued buffer
     */
    DataBuffer nextQueuedBuffer();
#ifdef QT_IS_AVAILABLE
signals:
    void streamingChanged(bool streaming);
#endif

protected:
    void handleNewDataRecieved(const google::protobuf::Message &message);
    void handleStopStreamRecieved(const google::protobuf::Message &message);
    void handleStartStreamRecieved(const google::protobuf::Message &message);

private:
    std::queue<DataBuffer> m_dataQueue;
    size_t totalBufferCount;
    bool m_isStreaming;
};

#endif // STREAMING_H
