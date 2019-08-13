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

	bool pause();

	bool stop();

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

	void serialLoop();
	void connectionLoop();

	bool establishConnection();

	void updateVoyagerConfiguration();

private:
	VoyagerHandle m_voyagerHandle;						/*!<The Comport Handle to the Voyager*/

	std::thread m_threadSerial;							/*!<This thread reads/writes serial data to/from the ComPort*/
	std::thread m_threadConnection;						/*!<This thread checks if the Voyager is disconnected*/

	std::vector<char> emptyBuffers;

	InterThreadStorage		m_interThreadStorage;		/*!<Contains the storage that can be reached from different threads*/
	EventsManager			m_eventsManager;			/*!<Handles the events*/
	ProtobufConfiguration	m_protobufConfiguration;	/*!<Contains the protobuf functions and setup*/
	SerialDriverInterface	m_serialDriverInterface;	/*!<Contains member functions to control the SerialInterface/connection with the Voyager */
};

