#include "InterThreadStorage.h"

InterThreadStorage::InterThreadStorage(EventsManager& events) : m_allowedToRun(false), m_eventsManager(&events)
{
}

void InterThreadStorage::set_AllowedToRun(bool state)
{
	std::lock_guard<std::mutex> lock(mutexAllowedToRun);
	m_allowedToRun = state;
}

const bool InterThreadStorage::allowedToRun()
{
	std::lock_guard<std::mutex> lock(mutexAllowedToRun);
	return m_allowedToRun;
}

const size_t InterThreadStorage::rawDataAvailable()
{
	std::lock_guard<std::mutex> lock(mutexRawSerialBuffer);
	return rawBuffer.size();
}

const size_t InterThreadStorage::parsedDataAvailable()
{
	std::lock_guard<std::mutex> lock(mutexParsedBuffer);
	return parsedBuffer.size();
}

const size_t InterThreadStorage::serialDataAvailableToSend()
{
	std::lock_guard<std::mutex> lock(mutexSerialDataToSend);
	return serialDataToSend.size();
}

void InterThreadStorage::appendRawSerialBuffer(const char* buffer, size_t size)
{
	std::lock_guard<std::mutex> lock(mutexRawSerialBuffer);
	rawBuffer.insert(rawBuffer.end(), buffer, buffer + size);
	m_eventsManager->call(DATARECEIVED);
}

void InterThreadStorage::appendParsedSerialBuffer(std::string buffer)
{
	std::lock_guard<std::mutex> lock(mutexParsedBuffer);
	parsedBuffer.insert(parsedBuffer.end(), buffer, buffer + size);
	m_eventsManager->call(DATAPARSED);
}

void InterThreadStorage::appendSerialDataToSend(const char* buffer, size_t size)
{
	serialDataToSend.insert(serialDataToSend.end(), buffer, buffer + size);
}

void InterThreadStorage::getRawSerialBuffer(char* buffer, size_t size)
{
	std::lock_guard<std::mutex> lock(mutexRawSerialBuffer);
	std::copy(rawBuffer.begin(), rawBuffer.begin() + size, buffer);
	rawBuffer.clear();
}

void InterThreadStorage::getParsedSerialBuffer(char* buffer, size_t size)
{
	std::lock_guard<std::mutex> lock(mutexParsedBuffer);
	std::copy(parsedBuffer.begin(), parsedBuffer.begin() + size, buffer);
	parsedBuffer.clear();
}

void InterThreadStorage::getSerialDataToSend(char* buffer, size_t size)
{
	std::lock_guard<std::mutex> lock(mutexSerialDataToSend);
	std::copy(serialDataToSend.begin(), serialDataToSend.begin() + size, buffer);
	serialDataToSend.clear();
}



