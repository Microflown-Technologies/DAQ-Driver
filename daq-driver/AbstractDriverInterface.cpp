#include "AbstractDriverInterface.h"

AbstractDriverInterface::AbstractDriverInterface() : m_connectionState(Disconnected)
{
}

AbstractDriverInterface::ConnectionState AbstractDriverInterface::connectionState() {
	return m_connectionState;
}

void AbstractDriverInterface::setVoyagerConnectedCallback(std::function<void(void)> callback) {
	if (callback) {
		m_voyagerConnectedCallback = callback;
	}
	else std::cerr << "Tried to set invalid callback in AbstractDriverInterface::setVoyagerConnectedCallback";
}

void AbstractDriverInterface::setConnectionStateChangeCallback(std::function<void(ConnectionState)> callback) {
	if (callback) {
		m_connectionStateChangedCallback = callback;
	} else std::cerr << "Tried to set invalid callback in AbstractDriverInterface::setConnectionStateChangeCallback";
}

void AbstractDriverInterface::setNewDataAvailableCallback(std::function<void(void)> callback) {
	if (callback) {
		m_newDataAvailableCallback = callback;
	}
	else std::cerr << "Tried to set invalid callback in AbstractDriverInterface::setDataRecievedCallback";
}

void AbstractDriverInterface::callVoyagerConnectedCallback() {
	if (m_voyagerConnectedCallback) m_voyagerConnectedCallback();
}

void AbstractDriverInterface::callConnectionStateChangedCallback(ConnectionState connectionState) {
	if (m_connectionStateChangedCallback) m_connectionStateChangedCallback(connectionState);
}

void AbstractDriverInterface::callNewDataAvailableCallback() {
	if (m_newDataAvailableCallback) m_newDataAvailableCallback();
}
