#include "InputRange.h"

InputRange::InputRange(pMessageProcessor messageProcessor) : AbstractDriverComponent(messageProcessor), m_inputRangeVoltages{_10}, m_actualInputRangeVoltages{10}
{
    MessageRouter::addMessageRoute<SetInputRange>(std::bind(&InputRange::handleSetInputRangeRecieved, this, std::placeholders::_1));
}

void InputRange::setInputRange(uint8_t channel, InputRange::Voltage voltage) {
     //Set internal value
    m_inputRangeVoltages[channel] = voltage;

    //Create and transmit Message
    SetInputRange newInputRange;
    newInputRange.set_channel(channel);
    newInputRange.set_voltage(static_cast<SetInputRange::Voltage>(getInputRange(channel)));
#ifdef QT_IS_AVAILABLE
    newInputRange.set_actualvoltage(static_cast<float>(getActualInputRange(channel)));
#endif
    m_messageProcessor->transmit(newInputRange);

    //Emit signal if QT is available
#ifdef QT_IS_AVAILABLE
    emit inputRangeChanged(channel, voltage);
#endif
}

void InputRange::setActualInputRange(uint8_t channel, float voltage) {
#ifdef QT_IS_AVAILABLE
    //Set internal value
    m_actualInputRangeVoltages[channel] = voltage;

    //Create and transmit Message
    SetInputRange newInputRange;
    newInputRange.set_channel(channel);
    newInputRange.set_voltage(static_cast<SetInputRange::Voltage>(getInputRange(channel)));
    newInputRange.set_actualvoltage(static_cast<float>(getActualInputRange(channel)));
    m_messageProcessor->transmit(newInputRange);
#endif
}

InputRange::Voltage InputRange::getInputRange(uint8_t channel) {
    return m_inputRangeVoltages[channel];
}

float InputRange::getActualInputRange(uint8_t channel) {
    return m_actualInputRangeVoltages[channel];
}

void InputRange::reset() {
    for(uint8_t channel = 0; channel < 6; channel++) {
        setInputRange(channel, _10);
#ifdef QT_IS_AVAILABLE
        emit inputRangeChanged(channel, _10);
#endif
    }
}

void InputRange::handleSetInputRangeRecieved(const google::protobuf::Message &message) {
    const SetInputRange& newInputRange = reinterpret_cast<const SetInputRange&>(message);
    //Set internal value for input range
    m_inputRangeVoltages[newInputRange.channel()] = static_cast<Voltage>(newInputRange.voltage());

#ifdef QT_IS_AVAILABLE
    emit inputRangeChanged(newInputRange.channel(), static_cast<Voltage>(newInputRange.voltage()));
#else
    //Set internal value for actual input range
    m_actualInputRangeVoltages[newInputRange.channel()] = static_cast<float>(newInputRange.actualvoltage());
    m_InputRangeRecievedCallbackHandler.invokeCallbacks();
#endif
}

std::shared_ptr<std::function<void ()> > InputRange::addInputRangeRecievedCallback(const std::function<void ()> &newBufferCallback) {
    return m_InputRangeRecievedCallbackHandler.addCallback(newBufferCallback);
}

bool InputRange::removeInputRangeRecievedCallback(std::shared_ptr<std::function<void ()> > callback) {
    return m_InputRangeRecievedCallbackHandler.removeCallback(callback);
}
