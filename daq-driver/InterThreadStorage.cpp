#include "InterThreadStorage.h"

InterThreadStorage::InterThreadStorage() : m_allowedToRun(false)
{
}

InterThreadStorage::~InterThreadStorage()
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

void InterThreadStorage::set_Connected(bool state)
{
	std::lock_guard<std::mutex> lock(mutexConnected);
	m_connected = state;
}

const bool InterThreadStorage::connected()
{
	std::lock_guard<std::mutex> lock(mutexConnected);
	return m_connected;
}

void InterThreadStorage::addVector(std::vector<char>* vector)
{
	std::lock_guard<std::mutex> lock(m_mutexEmptyBuffers);
	vector->clear();
	m_emptyBuffers.insert(m_emptyBuffers.end(), vector);
}

std::vector<char>* InterThreadStorage::getDataVector()
{
	std::vector<char>* buffer = m_filledBuffers.front();
	m_filledBuffers.erase(m_filledBuffers.begin());

	return buffer;
}

void InterThreadStorage::fillVector(char* buffer, size_t size)
{
	std::lock(m_mutexEmptyBuffers, m_mutexFilledBuffers);
	std::lock_guard<std::mutex> lock(m_mutexEmptyBuffers, std::adopt_lock);
	std::lock_guard<std::mutex> lock(m_mutexFilledBuffers, std::adopt_lock);

	m_emptyBuffers.front.resize(size);
	m_emptyBuffers.front = buffer;
	m_filledBuffers.insert(m_filledBuffers.end(), m_emptyBuffers.front());
	m_emptyBuffers.erase(m_emptyBuffers.begin());
}

bool InterThreadStorage::removeVector()
{
	std::lock_guard<std::mutex> lock(m_mutexEmptyBuffers);
	if (m_emptyBuffers.size()) {
		m_emptyBuffers.pop_back();
		return true;
	}
	else {
		std::cerr << "InterThreadStorage::removeVector: No empty buffers to delete";
		return false;
	}
}
