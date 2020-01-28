#ifndef FORMATTING_H
#define FORMATTING_H

//Internal headers
#include "AbstractDriverComponent.h"

//Protobuf
#include "DataFormat.pb.h"
#include "SetDataFormat.pb.h"

/**
 * @brief The Formatting class Handled anything related to the format of the streaming data
 */
class Formatting : public AbstractDriverComponent
{
#ifdef QT_IS_AVAILABLE
    Q_OBJECT
#endif
public:
    Formatting(MessageProcessor &messageProcessor);

    /**
     * @brief setDataFormat Sets a new data format
     * @param dataFormat the new dataformat
     */
    void setDataFormat(const DataFormat &dataFormat);

    /**
     * @brief dataFormat Gets the current data form
     * @return the current data format
     */
    DataFormat dataFormat() const;

    /**
     * @brief reset Resets the formatting to default
     */
    void reset();

#ifdef QT_IS_AVAILABLE
signals:
    void dataFormatChanged(const DataFormat &dataFormat);
#endif

protected:
    void handleNewFormatRecieved(const google::protobuf::Message &message);

private:
    DataFormat m_dataFormat;
};

#endif // FORMATTING_H
