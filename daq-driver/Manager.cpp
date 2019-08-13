#include "Manager.h"


Manager::Manager() : m_interThreadStorage(m_eventsManager), m_voyagerHandle(NULL)
{
	GOOGLE_PROTOBUF_VERIFY_VERSION;
}


Manager::~Manager()
{

}

void Manager::serialLoop()
{
	size_t size;
	char* buffer;

	while (m_interThreadStorage.allowedToRun()) {
		if (size = m_serialDriverInterface.bytesAvailable(m_voyagerHandle)) {
			
			buffer = new char[size];

			//Read the serialInterface of size bytes
			m_serialDriverInterface.read(m_voyagerHandle, buffer, size);

			delete[] buffer;

		}
	}
}

void Manager::connectionLoop()
{
	while (m_interThreadStorage.allowedToRun()) {
		if (!isVoyagerConnected()) {
			std::cout << "Voyager is disconnected" << std::endl;
			m_interThreadStorage.set_AllowedToRun(false);
			m_eventsManager.call(DISCONNECT);
		}
	}
}

void Manager::updateVoyagerConfiguration()
{
	size_t size = m_protobufConfiguration.ByteSizeLong();
	char* buffer = new char[size];

	//Serialize the protobuffer into an char * array
	m_protobufConfiguration.SerializeToArray(buffer, size);

	//Write new configuration
	m_serialDriverInterface.write(m_voyagerHandle, buffer, size);

	delete[] buffer;
}


void Manager::setEventCallback(std::function<void(Events)> callback)
{
	//Set the callback function
	m_eventsManager.setEventCallback(callback);
}

void Manager::clearEventCallback()
{
	//Clear the set callback function
	m_eventsManager.clearEventCallback();
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

bool Manager::establishConnection()
{
	//Check if a Callback function have been set and if so open a connection
	if (m_eventsManager.getEventCallback()) {
		m_voyagerHandle = m_serialDriverInterface.open(m_serialDriverInterface.isConnected());
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
	
	//Try to connect to the Voyager
	if (!establishConnection()) {
		std::cerr << "Was not able to establish connection" << std::endl;
		return false;
	}

	//Set allowedToRun to false and Voyager connected to true
	m_interThreadStorage.set_AllowedToRun(true);

	//Enable events and start the serial/connection threads
	m_eventsManager.enableEvents();
	m_threadSerial = std::thread(std::bind(&Manager::serialLoop, this));
	m_threadConnection = std::thread(std::bind(&Manager::connectionLoop, this));

	m_threadConnection.detach();
	m_threadSerial.detach();

	return true;
}

bool Manager::pause()
{
	//Disable events
	m_eventsManager.disableEvents();
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
	m_serialDriverInterface.close(m_voyagerHandle);

	//Delete the handle
	m_voyagerHandle = NULL;

	//Stop the events
	m_eventsManager.disableEvents();

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