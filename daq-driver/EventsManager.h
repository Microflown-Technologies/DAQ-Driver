#ifndef __EVENTSPUBLISHER_H__
#define __EVENTSPUBLISHER_H__

#include "szevent.h"

enum Events {
	CONNECT,
	DISCONNECT,
	DATARECEIVED,
	DATAPARSED,
	NEWCONFIGURATION,
	RAWDATAVECTOREMPTY,
	PARSEDDATAVECTOREMPTY,
	PROTBUFVECTOREMPTY,
	_ERROR
};

class EventsManager
{
public:
	EventsManager();
	virtual ~EventsManager();

	void call(Events eventje);

	void enableEvents();

	void disableEvents();

	std::function<void(Events)> getEventCallback();

	void setEventCallback(std::function<void(Events)> callback);

	void clearEventCallback();

private:

	bool m_enableEvents;

	sz::event<Events> voyagerEvent;
	std::function<void(Events)> m_eventCallback; /*!<Is called when a Voyager is connected to the system*/
};

#endif // __EVENTSPUBLISHER_H__