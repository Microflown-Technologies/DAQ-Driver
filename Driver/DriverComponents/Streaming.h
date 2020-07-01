#ifndef STREAMING_H
#define STREAMING_H

//STD Framework
#include <queue>
#include <memory>
#include <atomic>

//Internal headers
#include "AbstractDriverComponent.h"
#include "Threading/CallbackHandler.h"

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
    Streaming(pMessageProcessor messageProcessor);

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

    /**
     * @brief addNewBufferCallback Adds callback that is called when a new buffer is recieved
     * @param newBufferCallback callback to call when a new buffer is recieved
     */
    std::shared_ptr<std::function<void ()> > addNewBufferCallback(const std::function<void ()> &newBufferCallback);
    bool removeNewBufferCallback(std::shared_ptr<std::function<void(void)>> callback);

    std::shared_ptr<std::function<void ()> > addStreamStartedCallback(const std::function<void ()> &callback);
    bool removeStreamStartedCallback(std::shared_ptr<std::function<void(void)>> callback);

    std::shared_ptr<std::function<void ()> > addStreamStoppedCallback(const std::function<void ()> &callback);
    bool removeStreamStoppedCallback(std::shared_ptr<std::function<void(void)>> callback);


#ifdef QT_IS_AVAILABLE
signals:
    void streamingChanged(bool streaming);
#endif

protected:
    void handleNewDataRecieved(const google::protobuf::Message &message);
    void handleStopStreamRecieved(const google::protobuf::Message &message);
    void handleStartStreamRecieved(const google::protobuf::Message &message);

private:
    CallbackHandler m_streamStoppedCallbackHandler;
    CallbackHandler m_streamStartedCallbackHandler;
    CallbackHandler m_newBufferCallbackHandler;
    std::atomic<size_t> m_totalBufferCount;
    std::queue<DataBuffer> m_dataQueue;
    std::atomic_bool m_isStreaming;
    std::mutex m_dataQueueMutex;
};
typedef std::shared_ptr<Streaming> pStreaming;
#endif // STREAMING_H
