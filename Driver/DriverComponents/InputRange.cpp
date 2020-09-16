#include "InputRange.h"

InputRange::InputRange(pMessageProcessor messageProcessor) : AbstractDriverComponent(messageProcessor), m_inputRangeVoltages{_10}
{
    MessageRouter::addMessageRoute<SetInputRange>(std::bind(&InputRange::handleSetInputRangeRecieved, this, std::placeholders::_1));

}

void InputRange::setInputRange(uint8_t channel, InputRange::Voltage voltage) {
    //Create and transmit Message
    SetInputRange newInputRange;
    newInputRange.set_channel(channel);
    newInputRange.set_voltage(static_cast<SetInputRange::Voltage>(voltage));
    m_messageProcessor->transmit(newInputRange);
    //Set internal value and emit signal if QT is available
    m_inputRangeVoltages[channel] = voltage;
#ifdef QT_IS_AVAILABLE
    emit inputRangeChanged(channel, voltage);
#endif
}

InputRange::Voltage InputRange::getInputRange(uint8_t channel) {
    return m_inputRangeVoltages[channel];
}

void InputRange::reset() {
    for(uint8_t channel = 0; channel < 6; channel++) {
        setInputRange(channel, _10);
    }
}

void InputRange::handleSetInputRangeRecieved(const google::protobuf::Message &message)
{
    const SetInputRange& newInputRange = reinterpret_cast<const SetInputRange&>(message);
    m_inputRangeVoltages[newInputRange.channel()] = static_cast<Voltage>(newInputRange.voltage());
#ifdef QT_IS_AVAILABLE
    emit inputRangeChanged(newInputRange.channel(), static_cast<Voltage>(newInputRange.voltage()));
#endif
}

