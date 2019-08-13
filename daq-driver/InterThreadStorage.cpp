#include "InterThreadStorage.h"

InterThreadStorage::InterThreadStorage(EventsManager& events) : m_stopApplication(false), m_voyagerConnected(false), m_eventsManager(&events)
{
}

void InterThreadStorage::set_StopApplication(bool state)
{
	std::lock_guard<std::mutex> lock(mutexStopApplication);
	m_stopApplication = state;
}

void InterThreadStorage::set_VoyagerConnected(bool state)
{
	std::lock_guard<std::mutex> lock(mutexVoyagerConnected);
	m_voyagerConnected = state;
}

const bool InterThreadStorage::stopApplication()
{
	std::lock_guard<std::mutex> lock(mutexStopApplication);
	return m_stopApplication;
}

const bool InterThreadStorage::voyagerConnected()
{
	std::lock_guard<std::mutex> lock(mutexVoyagerConnected);
	return m_voyagerConnected;
}

const size_t InterThreadStorage::rawDataAvailable()
{
	std::lock_guard<std::mutex> lock(mutexRawSerialBuffer);
	return rawBufferCollection.size();
}

const size_t InterThreadStorage::parsedDataAvailable()
{
	std::lock_guard<std::mutex> lock(*mutexParsedSerialBuffer);
	return parsedSerialBuffer->size();
}

const size_t InterThreadStorage::serialDataAvailableToSend()
{
	std::lock_guard<std::mutex> lock(mutexSerialDataToSend);
	return serialDataToSend.size();
}

void InterThreadStorage::appendRawSerialBuffer(const char* buffer, size_t size)
{
	std::lock_guard<std::mutex> lock(mutexRawSerialBuffer);
	rawBufferCollection.insert(rawBufferCollection.end(), buffer, buffer + size);
	m_eventsManager->call(DATARECEIVED);
}

void InterThreadStorage::appendParsedSerialBuffer(const char* buffer, size_t size)
{
	std::lock_guard<std::mutex> lock(*mutexParsedSerialBuffer);
	parsedSerialBuffer->insert(parsedSerialBuffer->end(), buffer, buffer + size);
	m_eventsManager->call(DATAPARSED);
}

void InterThreadStorage::appendSerialDataToSend(const char* buffer, size_t size)
{
	serialDataToSend.insert(serialDataToSend.end(), buffer, buffer + size);
}

void InterThreadStorage::getRawSerialBuffer(char* buffer, size_t size)
{
	std::lock_guard<std::mutex> lock(mutexRawSerialBuffer);
	std::copy(rawBufferCollection.begin(), rawBufferCollection.begin() + size, buffer);
	rawBufferCollection.clear();
}

void InterThreadStorage::getParsedSerialBuffer(char* buffer, size_t size)
{
	std::lock_guard<std::mutex> lock(*mutexParsedSerialBuffer);
	std::copy(parsedSerialBuffer->begin(), parsedSerialBuffer->begin() + size, buffer);
	parsedSerialBuffer->clear();
}

void InterThreadStorage::getSerialDataToSend(char* buffer, size_t size)
{
	std::lock_guard<std::mutex> lock(mutexSerialDataToSend);
	std::copy(serialDataToSend.begin(), serialDataToSend.begin() + size, buffer);
	serialDataToSend.clear();
}



