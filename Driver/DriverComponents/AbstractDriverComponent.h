#ifndef ABSTRACTDRIVERCOMPONENT_H
#define ABSTRACTDRIVERCOMPONENT_H

#ifdef QT_IS_AVAILABLE
//Qt Framework
#include <QObject>
#endif

//Internal headers
#include "MessageProcessing/MessageProcessor.h"

/**
 * @brief The AbstractDriverComponent class Is the base class for all driver components hallo Bennie
 */
#ifdef QT_IS_AVAILABLE
class AbstractDriverComponent : public QObject
#else
class AbstractDriverComponent
#endif
{
#ifdef QT_IS_AVAILABLE
    Q_OBJECT
#endif
public:
    AbstractDriverComponent(MessageProcessor &messageProcessor);

    /**
     * @brief reset Resets the components
     */
    virtual void reset() = 0;

protected:
    MessageProcessor &m_messageProcessor;

};

#endif // ABSTRACTDRIVERCOMPONENT_H
