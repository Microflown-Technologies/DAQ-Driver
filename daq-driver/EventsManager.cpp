#include "EventsManager.h"
#include "SerialInterface/Windows/WindowsSerialInterface.h"

EventsManager::EventsManager() : m_enableEvents(false)
{

	sz::event_handler<Events> eventHandler([this](Events eventje) {
		m_eventCallback(eventje);
		});

	voyagerEvent += eventHandler;
}

EventsManager::~EventsManager()
{

}

void EventsManager::call(Events eventje)
{
	if (m_enableEvents) {
		voyagerEvent(eventje);
	}
	else {
		std::cerr << "Event called but not enabled" << std::endl;
	}
}

void EventsManager::enableEvents()
{
	m_enableEvents = true;
}

void EventsManager::disableEvents()
{
	m_enableEvents = false;
}

std::function<void(Events)> EventsManager::getEventCallback()
{
	return m_eventCallback;
}

void EventsManager::setEventCallback(std::function<void(Events)> callback)
{
	if (callback) {
		m_eventCallback = callback;
	}
	else std::cerr << "Tried to set invalid callback in Manager::setEventCallback";
}

void EventsManager::clearEventCallback() {
	m_eventCallback = NULL;
}
