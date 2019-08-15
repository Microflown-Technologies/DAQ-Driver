#ifndef __EVENTSPUBLISHER_H__
#define __EVENTSPUBLISHER_H__

#include "szevent.h"

enum Events {
	CONNECT,
	DISCONNECT,
	DATARECEIVED,
	DATAPARSED,
	DATAREADY,
	NEWCONFIGURATION,
	RAWDATAVECTOREMPTY,
	PARSEDDATAVECTOREMPTY,
	PROTBUFVECTOREMPTY,
	_ERROR
};

class EventManager
{
public:
	EventManager();
	virtual ~EventManager();

	void throwLibraryEvent(Events eventje);

	void throwHardwareEvent();

	void throwDataEvent();

	void enableEvents();

	void disableEvents();

	std::function<void(Events)> getEventCallback();

	void setLibraryEventCallback(std::function<void(Events)> callback);

	void setHardwareEventCallback(std::function<void()> callback);

	void setDataEventCallback(std::function<void()> callback);

	void clearEventCallback();

private:

	bool m_enableEvents;

	sz::event<Events> libraryEvent;							/*!<Event which is thrown for cummunication with the user of this library*/
	sz::event<>	hardwareEvent;								/*!<Event which is thrown when a hardware change has occured*/
	sz::event<> dataEvent;									/*!<Event which is thrown when data is availlable*/

	std::function<void()> m_hardwareEventCallback;			/*!<Is called when the serialInterface notices a change in hardware*/
	std::function<void()> m_dataEventCallback;				/*!<Is called when the serialInterface sends an event*/
	std::function<void(Events)> m_libraryEventCallback;		/*!<Is called when a the library sends an event*/
};

#endif // __EVENTSPUBLISHER_H__