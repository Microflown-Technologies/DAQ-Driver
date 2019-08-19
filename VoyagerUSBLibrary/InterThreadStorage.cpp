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

void InterThreadStorage::addVector(std::string* string)
{
	std::lock_guard<std::mutex> lock(m_mutexEmptyBuffers);
	string->clear();
	m_emptyBuffers.insert(m_emptyBuffers.end(), string);
}

std::string* InterThreadStorage::getDataVector()
{
	if (!m_filledBuffers.size()) {
		std::cerr << "Tried to retrieve data but there are no vectors to retrieve from" << std::endl;
		return NULL;
	}
	std::string* buffer = m_filledBuffers.front();
	m_filledBuffers.erase(m_filledBuffers.begin());

	return buffer;
}

bool InterThreadStorage::fillVector(std::string &string, size_t size)
{
	m_mutexEmptyBuffers.lock();
	m_mutexFilledBuffers.lock();

	if (!m_emptyBuffers.size()) {
		m_mutexFilledBuffers.unlock();
		m_mutexEmptyBuffers.unlock();
		std::cerr << "InterThreadStorage::fillVector: No empty buffers to fill" << std::endl;
		return false;
	}
	m_emptyBuffers.front()->reserve(size);
	const auto vector = m_emptyBuffers.front();

	for (int i = 0; i < size; i++) {
		vector->push_back(string[i]);
	}

	m_filledBuffers.insert(m_filledBuffers.end(), m_emptyBuffers.front());
	m_emptyBuffers.erase(m_emptyBuffers.begin());

	m_mutexFilledBuffers.unlock();
	m_mutexEmptyBuffers.unlock();

	return true;

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
