#include "Manager.h"


Manager::Manager() : m_serialDriverInterface(&m_eventManager, &m_interThreadStorage), m_voyagerHandle(NULL)
{
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	m_protobufComparer.addIgnoreField("audioData");
	m_protobufComparer.addIgnoreField("timestamp");

	m_eventManager.setDataEventCallback(std::bind(&Manager::dataHandler, this));
	m_eventManager.setHardwareEventCallback(std::bind(&Manager::hardwareHandler, this));
}


Manager::~Manager()
{
	m_interThreadStorage.set_AllowedToRun(false);
	stop();
}

void Manager::dataHandler()
{
	size_t size;
	char* buffer;

	if ((size = m_serialDriverInterface.bytesAvailable(m_voyagerHandle, &m_voyagerHandleMutex))) {

		//Notify the callback function that data is received
		m_eventManager.throwLibraryEvent(DATARECEIVED);

		//Allocate memory for the data that is about to be read
		buffer = new char[size];

		//Read the serialInterface of size bytes
		m_voyagerHandleMutex.lock();
		m_serialDriverInterface.read(m_voyagerHandle, buffer, size);
		m_voyagerHandleMutex.unlock();

		//Parse the serial data to the Protobuf format
		m_protobufConfigurationBuf = m_protobufParser.parseSerialData(buffer, size);

		//Compare the received protobuffer with the saved protobuffer
		auto futureCompareResult = std::async(std::launch::async, std::bind(&ProtobufComparer::compareProtobufs, &m_protobufComparer, m_protobufConfiguration, m_protobufConfigurationBuf));

		//auto compareResult = m_protobufComparer.compareProtobufs(m_protobufConfiguration, m_protobufConfigurationBuf);

		size_t wow = m_protobufConfigurationBuf.audiodata().size();


		//Write the parsed data to the ParsedBuffer
		if (!m_interThreadStorage.fillVector(*m_protobufConfigurationBuf.mutable_audiodata(), wow)) {
			m_eventManager.throwLibraryEvent(ALLBUFFERSUSED);
		}
		m_eventManager.throwLibraryEvent(DATAREADY);

		//Wait til the protobuffers are compared
		futureCompareResult.wait();


		if (!m_protobufConfiguration.samplingfrequency()) {
			m_protobufConfiguration = m_protobufConfigurationBuf;
			delete[] buffer;
			return;
		}

		if (!futureCompareResult.get()) {
			m_protobufConfiguration = m_protobufConfigurationBuf;
			m_eventManager.throwLibraryEvent(NEWCONFIGURATION);
		}

		delete[] buffer;
	}
}

void Manager::hardwareHandler()
{
	if (!isVoyagerConnected()) {
		std::cout << "Voyager is disconnected" << std::endl;
		m_interThreadStorage.set_AllowedToRun(false);
		m_eventManager.throwLibraryEvent(DISCONNECT);
	}
	else {
		std::cout << "Voyager is connected" << std::endl;
		m_interThreadStorage.set_AllowedToRun(true);
		m_eventManager.throwLibraryEvent(CONNECT);
	}
}

void Manager::updateVoyagerConfiguration()
{
	size_t size = m_protobufConfiguration.ByteSizeLong();
	char* buffer = new char[size];

	//Serialize the protobuffer into an char * array
	m_protobufConfiguration.SerializeToArray(buffer, size);

	//Write new configuration
	m_voyagerHandleMutex.lock();
	m_serialDriverInterface.write(m_voyagerHandle, buffer, size);
	m_voyagerHandleMutex.unlock();
	delete[] buffer;
}


void Manager::setLibraryEventCallback(std::function<void(Events)> callback)
{
	//Set the callback function
	m_eventManager.setLibraryEventCallback(callback);
}

void Manager::clearEventCallback()
{
	//Clear the set callback function
	m_eventManager.clearEventCallback();
}

bool Manager::isVoyagerConnected()
{
	//Check if the Voyager is connected 
	if (m_serialDriverInterface.isConnected() == "") {
		return false;
	}
	else {
		return true;
	}
}

void Manager::provideStorageVector(std::string* bufferVector)
{
	m_interThreadStorage.addVector(bufferVector);
}

void Manager::provideStorageVector(std::vector<std::string>* bufferVectors) {
	for (auto& vector : *bufferVectors) {
		m_interThreadStorage.addVector(&vector);
	}
}

std::string* Manager::getData()
{
	return m_interThreadStorage.getDataVector();
}

bool Manager::establishConnection()
{
	//Check if a Callback function have been set and if so open a connection
	if (m_eventManager.getEventCallback()) {
		m_voyagerHandleMutex.lock();
		m_voyagerHandle = m_serialDriverInterface.open(m_serialDriverInterface.isConnected());
		m_voyagerHandleMutex.unlock();
		return true;
	}
	else {
		std::cerr << "Can't establish connection before callback is set" << std::endl;
		return false;
	}
}

bool Manager::disconnect() {
	//Close the connection
	m_serialDriverInterface.close(m_voyagerHandle);
	return false;
}

bool Manager::start()
{
	//Check if there is a Voyager which can be read
	if (!isVoyagerConnected()) {
		std::cerr << "Start called but no Voyager is connected" << std::endl;
		return false;
	}

	//Enable events
	m_eventManager.enableEvents();
	
	//Try to connect to the Voyager
	if (!establishConnection()) {
		std::cerr << "Was not able to establish connection" << std::endl;
		return false;
	}

	//Set allowedToRun to false and Voyager connected to true
	m_interThreadStorage.set_AllowedToRun(true);

	//start the serial/connection threads
	m_serialDriverInterface.dataAvailableLoop(m_voyagerHandle, m_voyagerHandleMutex);
	return true;
}

bool Manager::pause()
{
	//Disable events
	m_eventManager.disableEvents();
	return true;
}

bool Manager::stop()
{
	//Send a signal to the threads to stop
	m_interThreadStorage.set_AllowedToRun(false);

	//Wait till threads are closed
	//m_threadSerial.join();
	//m_threadConnection.join();

	//Close the connection with the SerialInterface
	m_voyagerHandleMutex.lock();
	m_serialDriverInterface.close(m_voyagerHandle);
	

	//Delete the handle
	m_voyagerHandle = NULL;
	m_voyagerHandleMutex.unlock();

	//Stop the events
	m_eventManager.disableEvents();

	return true;
}

/***************************************************************************************************************************************************************************/
/********************************************************************-----------------------********************************************************************************/
/********************************************************************|  GETTERS & SETTERS  |********************************************************************************/
/********************************************************************-----------------------********************************************************************************/
/***************************************************************************************************************************************************************************/

uint32_t Manager::getSamplingFrequency() const
{
	return m_protobufConfiguration.samplingfrequency();
}

uint32_t Manager::getTDMMode() const
{
	return m_protobufConfiguration.tdmmode();
}

uint32_t Manager::getNrOfChannels() const
{
	return m_protobufConfiguration.nrofchannels();
}

uint32_t Manager::getSelectedChannels() const
{
	return m_protobufConfiguration.selectedchannels();
}

uint32_t Manager::getMsAudio() const
{
	return m_protobufConfiguration.msaudio();
}

uint32_t Manager::getFrameSize() const
{
	return m_protobufConfiguration.framesize();
}

uint32_t Manager::getSamplesPerChannel() const
{
	return m_protobufConfiguration.samplesperchannel();
}

uint32_t Manager::getBytesPerSample() const
{
	return m_protobufConfiguration.bytespersample();
}

uint32_t Manager::getBytesPerChannel() const
{
	return m_protobufConfiguration.bytesperchannel();
}

uint64_t Manager::getTimeStamp() const
{
	return m_protobufConfiguration.timestamp();
}

uint64_t Manager::getSampleIncrement() const 
{
	return uint64_t();
}

Gain Manager::getGain(Input input)
{
	switch (input) {
	case Aux1:
		return m_protobufConfiguration.mutable_bnc1()->gain();
		break;
	case Aux2:
		return m_protobufConfiguration.mutable_bnc2()->gain();
		break;
	case Flown:
		return m_protobufConfiguration.mutable_lemo()->gain();
		break;
	default:
		std::cerr << "getGain: Input type is not availlable" << std::endl;
		return GAINNOTUSED;
	}
}

Iepe Manager::getIEPE(Input input)
{
	switch (input) {
	case Aux1:
		return m_protobufConfiguration.mutable_bnc1()->iepe();
		break;
	case Aux2:
		return m_protobufConfiguration.mutable_bnc2()->iepe();
		break;
	case Flown:
		std::cerr << "The flown does not have IEPE setting." << std::endl;
		return IEPENOTUSED;
		break;
	default:
		std::cerr << "GetIEPE: Input type is not availlable" << std::endl;
		return IEPENOTUSED;
	}
}

void Manager::setSamplingFrequency(uint32_t samplingfrequency)
{
	if (samplingfrequency == 8000 || samplingfrequency == 16000 || samplingfrequency == 24000 || samplingfrequency == 32000 || samplingfrequency == 48000 && samplingfrequency != 5) {
		m_protobufConfiguration.set_samplingfrequency(samplingfrequency);
		
		if (m_interThreadStorage.allowedToRun()) {
			std::async(std::bind(&Manager::updateVoyagerConfiguration, this));
		}	
	}
}

void Manager::setGain(Gain gain, Input input) {
	switch (input) {
	case Aux1:
		m_protobufConfiguration.mutable_bnc1()->set_gain(gain);
		break;
	case Aux2:
		m_protobufConfiguration.mutable_bnc2()->set_gain(gain);
		break;
	case Flown:
		m_protobufConfiguration.mutable_lemo()->set_gain(gain);
		break;
	default:
		std::cerr << "setGain: Input type is not availlable" << std::endl;
		return;
	}
	if (m_interThreadStorage.allowedToRun()) {
		std::async(std::bind(&Manager::updateVoyagerConfiguration, this));
	}
}

void Manager::setIEPE(Iepe iepe, Input input) {
	if (input == IEPENOTUSED) return;

	switch (input) {
	case Aux1:
		m_protobufConfiguration.mutable_bnc1()->set_iepe(iepe);
		break;
	case Aux2:
		m_protobufConfiguration.mutable_bnc2()->set_iepe(iepe);
		break;
	case Flown:
		std::cerr << "The flown does not have IEPE settings that can be set." << std::endl;
		break;
	default:
		std::cerr << "setIEPE: Input type is not availlable" << std::endl;
		return;
	}
	if (m_interThreadStorage.allowedToRun()) {
		std::async(std::bind(&Manager::updateVoyagerConfiguration, this));
	}
}