#define NOMINMAX
#include <thread>

#ifdef _WIN32
#include "SerialInterface/Windows/WindowsSerialInterface.h"
#define SerialDriverInterface WindowsSerialInterface
#elif _VOYAGER
#include "VoyagerSerialInterface.h"
#define SerialDriverInterface VoyagerSerialInterface
#else
#include "LinuxSerialInterface.h"
#define SerialDriverInterface LinuxSerialInterface
#endif 


#include "EventsManager.h"
#include "Protobuf/ProtobufParser.h"


#pragma once
class Manager
{
public:
	Manager();
	~Manager();

	/**
	* @details setConnectionStateChangeCallback Sets a callback function that is called when the state of connection between	
	* the Voyager and the PC changes
	* @param callback function pointer to callback
	**/
	void setEventCallback(std::function<void(Events)> callback);

	void clearEventCallback();

	bool isVoyagerConnected();

	bool disconnect();

	void addRawDataBuffer(std::vector<char>* buffer);

	void addRawDataBuffer(std::vector<std::vector<char>>* buffers);

	void addParsedDataBuffer(std::vector<char>* buffer);

	void addParsedDataBuffer(std::vector<std::vector<char>>* buffers);

	void addProtobufBuffer(std::vector<ProtobufConfiguration>* proBuf);

	void addProtobufBuffer(std::vector<std::vector<ProtobufConfiguration>>* proBuffs);

	bool start();

	void pause();

	void stop();

	uint32_t getSamplingFrequency() const;

	uint32_t getTDMMode() const;

	uint32_t getNrOfChannels() const;
	
	uint32_t getSelectedChannels() const;

	uint32_t getMsAudio() const;

	uint32_t getFrameSize() const;

	uint32_t getSamplesPerChannel() const;

	uint32_t getBytesPerSample() const;

	uint32_t getBytesPerChannel() const;

	uint64_t getTimeStamp() const;

	uint64_t getSampleIncrement() const;
	
	Gain getGain(Input input);

	Iepe getIEPE(Input input);

	void setSamplingFrequency(uint32_t samplingfrequency);
	
	void setGain(Gain gain, Input input);

	void setIEPE(Iepe iepe, Input input);

protected:


	void serialLoop(int x);


	bool establishConnection();

	void updateVoyagerConfiguration();

private:
	
	VoyagerHandle m_voyagerHandle;


	std::thread m_threadSerial;

	InterThreadStorage		m_interThreadStorage;
	EventsManager			m_eventsManager;
	ProtobufConfiguration	m_protobufConfiguration;
	SerialDriverInterface	m_serialDriverInterface;
};

