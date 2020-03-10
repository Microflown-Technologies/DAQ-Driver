#ifndef ABSTRACTDRIVERCOMPONENT_H
#define ABSTRACTDRIVERCOMPONENT_H

#ifdef QT_IS_AVAILABLE
//Qt Framework
#include <QObject>
#endif

//STD framework
#include <memory>

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
    AbstractDriverComponent(pMessageProcessor messageProcessor);

    /**
     * @brief reset Resets the components
     */
    virtual void reset() = 0;

protected:
    pMessageProcessor m_messageProcessor;
};
typedef std::shared_ptr<AbstractDriverComponent> pAbstractDriverComponent;
#endif // ABSTRACTDRIVERCOMPONENT_H
