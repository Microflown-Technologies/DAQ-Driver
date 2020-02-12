#ifndef IEPE_H
#define IEPE_H

//STD framework
#include <atomic>
#include <stdint.h>

//Internal headers
#include "AbstractDriverComponent.h"

//Protobuf
#include "SetIEPE.pb.h"
#include <google/protobuf/message.h>

/**
 * @brief The IEPE class Can be used to control IEPE power on the AUX Channels
 */
class IEPE : public AbstractDriverComponent
{
#ifdef QT_IS_AVAILABLE
    Q_OBJECT
#endif
public:
    typedef enum {Aux1 = SetIEPE::Aux1,
                  Aux2 = SetIEPE::Aux2} Input;
    explicit IEPE(MessageProcessor &messageProcessor);

    /**
     * @brief setIEPE Enables or disables IEPE for channel
     * @param channel the Channel to enable or disable IEPE power for
     * @param enabled true is enabled, false is disabled
     */
    void setIEPE(Input channel, bool enabled);

    /**
     * @brief getIEPE Gets if IEPE is enabled for a channel
     * @param channel channel to check for
     * @return returns true if enabled, false if disabled
     */
    bool getIEPE(Input channel);

    /**
     * @brief reset Resets IEPE to default values
     */
    void reset() override;

#ifdef QT_IS_AVAILABLE
signals:
    void iepeChanged(Input input, bool enabled);
#endif

protected:
    void handleSetIEPERecieved(const google::protobuf::Message &message);

private:
    std::atomic_bool m_iepeEnabled[2];
};






#endif // IEPE_H
