#pragma once

#include "EventsManager.h"
#include <vector>
#include <mutex>

class InterThreadStorage {
public:
	InterThreadStorage(EventsManager &events);

	void set_StopApplication(bool state);

	void set_VoyagerConnected(bool state);

	const bool stopApplication();

	const bool voyagerConnected();
	
	const size_t rawDataAvailable();

	const size_t parsedDataAvailable();

	const size_t serialDataAvailableToSend();

	void appendRawSerialBuffer(const char* buffer, size_t size);

	void appendParsedSerialBuffer(const char* buffer, size_t size);

	void appendSerialDataToSend(const char* buffer, size_t size);

	void getRawSerialBuffer(char* buffer, size_t size);

	void getParsedSerialBuffer(char* buffer, size_t size);

	void getSerialDataToSend(char* buffer, size_t size);

private:

	bool m_stopApplication;
	std::mutex mutexStopApplication;

	bool m_voyagerConnected;
	std::mutex mutexVoyagerConnected;

	std::vector<char> rawBuffer;
	std::mutex mutexRawSerialBuffer;

	std::vector<char> serialDataToSend;
	std::mutex mutexSerialDataToSend;

	std::vector<char> parsedBuffer;
	std::mutex mutexParsedSerialBuffer;

	EventsManager* m_eventsManager;
};