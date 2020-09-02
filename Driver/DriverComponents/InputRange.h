#ifndef INPUTRANGE_H
#define INPUTRANGE_H

//STD framework
#include <atomic>
#include <memory>
#include <cstdint>

//Internal headers
#include "AbstractDriverComponent.h"

//Protobuf
#include "SetInputRange.pb.h"
#include <google/protobuf/message.h>

/**
 * @brief The InputRange class Handles controlling the inputrange (V) for induvidual channels
 */
class InputRange : public AbstractDriverComponent
{
#ifdef QT_IS_AVAILABLE
    Q_OBJECT
#endif
public:
    typedef enum {_0_1  = SetInputRange::_0_1,
                  _1    = SetInputRange::_1,
                  _10   = SetInputRange::_10} Voltage;
    explicit InputRange(pMessageProcessor messageProcessor);


    /**
     * @brief setInputRange Sets inputrange for channel
     * @param channel channel to set inputrange for
     * @param voltage input range in Volts
     */
    void setInputRange(uint8_t channel, Voltage voltage);
    /**
     * @brief setInputRange Sets inputrange for channel
     * @param channel channel to set inputrange for
     * @param voltage input range in Volts
     */
    void setActualInputRange(uint8_t channel, float voltage);
    /**
     * @brief getInputRange Gets input range for channel in Volts
     * @param channel channel to get inputrange for
     * @return inputrange in volts
     */
    Voltage getInputRange(uint8_t channel);

    /**
     * @brief getInputRange Gets input range for channel in Volts
     * @param channel channel to get inputrange for
     * @return inputrange in volts
     */
    float getActualInputRange(uint8_t channel);

    void reset() override;

#ifdef QT_IS_AVAILABLE
signals:
    void inputRangeChanged(uint8_t channel, Voltage voltage);
#endif

protected:
    void handleSetInputRangeRecieved(const google::protobuf::Message &message);

private:
    std::atomic<Voltage> m_inputRangeVoltages[6];
    std::atomic<float> m_actualInputRangeVoltages[6];

};
typedef std::shared_ptr<InputRange> pInputRange;

#endif // INPUTRANGE_H
