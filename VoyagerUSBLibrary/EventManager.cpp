#include "EventManager.h"
#include "SerialInterface/Windows/WindowsSerialInterface.h"

EventManager::EventManager() : m_enableEvents(false)
{
	sz::event_handler<Events> libraryEventHandler([this](Events eventje) {
		m_libraryEventCallback(eventje);
	});
	
	sz::event_handler<> hardwareEventHandler([this]() {
		m_hardwareEventCallback();
	});
	
	sz::event_handler<> dataEventHandler([this]() {
		m_dataEventCallback();
	});
	
	libraryEvent += libraryEventHandler;
	hardwareEvent += hardwareEventHandler;
	dataEvent += dataEventHandler;
}

EventManager::~EventManager()
{
}

void EventManager::throwLibraryEvent(Events eventje)
{
	if (m_enableEvents) {
		libraryEvent(eventje);
	}
	else {
		std::cerr << "Event called but not enabled" << std::endl;
	}
}

void EventManager::throwHardwareEvent()
{
	hardwareEvent();
}

void EventManager::throwDataEvent()
{
	dataEvent();
}

void EventManager::enableEvents()
{
	m_enableEvents = true;
}

void EventManager::disableEvents()
{
	m_enableEvents = false;
}

std::function<void(Events)> EventManager::getEventCallback()
{
	return m_libraryEventCallback;
}

void EventManager::setLibraryEventCallback(std::function<void(Events)> callback)
{
	if (callback) {
		m_libraryEventCallback = callback;
	}
	else std::cerr << "Tried to set invalid callback in Manager::setEventCallback";
}

void EventManager::setHardwareEventCallback(std::function<void()> callback)
{
	if (callback) {
		m_hardwareEventCallback = callback;
	}
	else std::cerr << "Tried to set invalid callback in Manager::setEventCallback";
}

void EventManager::setDataEventCallback(std::function<void()> callback)
{
	if (callback) {
		m_dataEventCallback = callback;
	}
	else std::cerr << "Tried to set invalid callback in Manager::setEventCallback";
}

void EventManager::clearEventCallback() {
	m_libraryEventCallback = NULL;
}
