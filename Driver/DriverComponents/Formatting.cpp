#include "Formatting.h"

Formatting::Formatting(MessageProcessor &messageProcessor) : AbstractDriverComponent(messageProcessor)
{
    MessageRouter::addMessageRoute<SetDataFormat>(std::bind(&Formatting::handleNewFormatRecieved, this, std::placeholders::_1));
    reset();
}

void Formatting::setDataFormat(const DataFormat &dataFormat) {
    SetDataFormat newFormat;
    swapDataFormat(dataFormat);
    newFormat.set_allocated_newformat(new DataFormat(m_dataFormat));
    m_messageProcessor.transmit(newFormat);
}

DataFormat Formatting::dataFormat() const {
    return m_dataFormat;
}

void Formatting::reset() {
    // Set Format
    DataFormat newFormat;
    newFormat.set_samplerate(DataFormat::SampleRate::DataFormat_SampleRate__48000);
    setDataFormat(newFormat);
}

void Formatting::handleNewFormatRecieved(const google::protobuf::Message &message) {
    m_dataFormat.CopyFrom(dynamic_cast<const SetDataFormat&>(message).newformat());
#ifdef QT_IS_AVAILABLE
    emit dataFormatChanged(m_dataFormat);
#endif
}

void Formatting::swapDataFormat(const DataFormat &newFormat) {
    const std::lock_guard<std::mutex> gaurd(m_dataFormatMutex);
    m_dataFormat.CopyFrom(newFormat);
}
