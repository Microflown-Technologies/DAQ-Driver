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

#include "InterThreadStorage.h"
#include "EventManager.h"

#include "Protobuf/ProtobufParser.h"
#include "Protobuf/ProtobufComparer.h"



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
	void setLibraryEventCallback(std::function<void(Events)> callback);

	/** 
	* @details Clear the callback pointer so no events will arive
	**/
	void clearEventCallback();

	/**
	* @ details
	* @ return bool TRUE if the Voyager is connected FALSE if not
	**/
	bool isVoyagerConnected();


	void provideStorageVector(std::string *bufferVector);

	void provideStorageVector(std::vector<std::string> *bufferVectors);

	std::string* getData();

	/**
	* @details Starts sending events start communication with the connected device
	* @return bool TRUE if sucess and FALSE if not
	**/
	bool start();
	
	/**
	* @details Stops the sending of events
	* @return bool TRUE if sucess and FALSE if not
	**/
	bool pause();

	/**
	* @details Stops the sending of events and closes the communication with the connected device
	* @return bool TRUE if success and FALSE if not
	**/
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

	/**
	* @details Returns the Gain setting of the requested input
	* @param Input AUX1 / AUX2 / FLOWN
	* @return Gain LOW / MEDIUM / HIGH
	**/
	Gain getGain(Input input);

	/**
	* @details Returns the IEPE setting of the requested input
	* @param Input AUX1 / AUX2
	* @return IEPE ON / OFF
	**/
	Iepe getIEPE(Input input);

	/**
	* @details set the SamplingFrequency for the Protobuf object in this class and synchronizes it with the connected device.
	* @param samplingfrequency 48000 / 32000 / 24000 / 16000 / 8000
	**/
	void setSamplingFrequency(uint32_t samplingfrequency);

	/**
	* @details Sets the Gain setting for the Protobuf object in this class and synchronizes it with the connected device.
	* @param Gain LOW / MEDIUM / HIGH
	* @param Input AUX1 / AUX2 / FLOWN
	**/
	void setGain(Gain gain, Input input);

	/**
	* @details Sets the IEPE setting for the Protobuf object of this class and synchronizes it with the connected device.
	* @param IEPE ON / OFF
	* @param Input AUX1 / AUX2
	**/
	void setIEPE(Iepe iepe, Input input);

protected:
	/**
	* @details
	* @return bool TRUE if success FALSE if not 
	**/
	bool disconnect();

	/**
	* @details Handles the data event thrown by SerialInterfaceDriver::dataLoop
	**/
	void dataHandler();

	/**
	* @details Handles the hardware event thrown by SerialInterfaceDriver::hardwareLoop
	**/
	void hardwareHandler();

	/**
	* @details connects the SerialInterface Driver to the connected device
	**/
	bool establishConnection();

	/**
	* @details Called after the configuration is changed, sends the configuration to the connected device
	**/
	void updateVoyagerConfiguration();

private:
	VoyagerHandle m_voyagerHandle;						/*!<The Comport Handle to the Voyager*/
	std::mutex m_voyagerHandleMutex;					/*!<Mutex for ensuring that the handle is not used at the same time*/

	std::thread m_threadSerial;							/*!<This thread reads/writes serial data to/from the ComPort*/
	std::thread m_threadConnection;						/*!<This thread checks if the Voyager is disconnected*/

	std::vector<char> emptyBuffers;

	InterThreadStorage		m_interThreadStorage;		/*!<Contains the storage that can be reached from different threads*/
	EventManager			m_eventManager;				/*!<Handles the events*/

	ProtobufConfiguration	m_protobufConfigurationBuf; /*!<Used as a buffer for comparison with the current configuration*/
	ProtobufConfiguration	m_protobufConfiguration;	/*!<Contains the protobuf functions and setup*/
	ProtobufComparer		m_protobufComparer;			/*!<Class to compare two protobufs*/
	ProtobufParser			m_protobufParser;			/*!<Class to parse serial data to a protobuf and the other way around*/

	SerialDriverInterface	m_serialDriverInterface;	/*!<Contains member functions to control the SerialInterface/connection with the Voyager */
};

