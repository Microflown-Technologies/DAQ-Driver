#include "Formatting.h"

Formatting::Formatting(MessageProcessor &messageProcessor) : AbstractDriverComponent(messageProcessor)
{
    MessageRouter::addMessageRoute<SetDataFormat>(std::bind(&Formatting::handleNewFormatRecieved, this, std::placeholders::_1));
    reset();
}

void Formatting::setDataFormat(const DataFormat &dataFormat) {
    SetDataFormat newFormat;
    m_dataFormat.CopyFrom(dataFormat);
    newFormat.set_allocated_newformat(new DataFormat(m_dataFormat));
    m_messageProcessor.transmit(newFormat);
}

DataFormat Formatting::dataFormat() const {
    return m_dataFormat;
}

void Formatting::reset()
{
    // Set Format
    m_dataFormat.set_samplerate(DataFormat::SampleRate::DataFormat_SampleRate__48000);
    setDataFormat(m_dataFormat);
}

void Formatting::handleNewFormatRecieved(const google::protobuf::Message &message) {
    m_dataFormat.CopyFrom(dynamic_cast<const SetDataFormat&>(message).newformat());
#ifdef QT_IS_AVAILABLE
    emit dataFormatChanged(m_dataFormat);
#endif
}
