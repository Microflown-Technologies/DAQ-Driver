#include "IEPE.h"

IEPE::IEPE(pMessageProcessor messageProcessor) : AbstractDriverComponent(messageProcessor), m_iepeEnabled{false}
{
    MessageRouter::addMessageRoute<SetIEPE>(std::bind(&IEPE::handleSetIEPERecieved, this, std::placeholders::_1));

}

void IEPE::setIEPE(IEPE::Input channel, bool enabled) {
    //Create message and send message
    SetIEPE setIEPEMessage;
    setIEPEMessage.set_input(static_cast<SetIEPE_Input>(channel));
    setIEPEMessage.set_enabled(enabled);
    m_messageProcessor->transmit(setIEPEMessage);
    //Set variables
    m_iepeEnabled[channel] = enabled;
#ifdef QT_IS_AVAILABLE
    emit iepeChanged(channel, enabled);
#endif
}

bool IEPE::getIEPE(IEPE::Input channel) {
    return m_iepeEnabled[channel];
}

void IEPE::reset() {
    setIEPE(Aux1, false);
    setIEPE(Aux2, false);
}

void IEPE::handleSetIEPERecieved(const google::protobuf::Message &message)
{
    const SetIEPE& newIEPESetting = dynamic_cast<const SetIEPE&>(message);
    m_iepeEnabled[newIEPESetting.input()] = newIEPESetting.enabled();
#ifdef QT_IS_AVAILABLE
    emit iepeChanged(static_cast<IEPE::Input>(newIEPESetting.input()), newIEPESetting.enabled());
#endif
}
