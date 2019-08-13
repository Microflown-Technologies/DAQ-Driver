#include "Manager.h"

Manager::Manager() : m_interThreadStorage(m_eventsManager), m_voyagerHandle(NULL)
{


	GOOGLE_PROTOBUF_VERIFY_VERSION;

}

Manager::~Manager()
{

}


void Manager::serialLoop(int x)
{
	size_t size;


	while (!m_interThreadStorage.stopApplication() && m_interThreadStorage.voyagerConnected()) {

		if (size = m_serialDriverInterface.bytesAvailable(m_voyagerHandle)) {
			




		}
	}
}

void Manager::updateVoyagerConfiguration()
{
	size_t size = m_protobufConfiguration.ByteSizeLong();
	char* buffer = new char[size];

	//Serialize the protobuffer into an char * array
	m_protobufConfiguration.SerializeToArray(buffer, size);

	//Append the serialzed protobuffer to the buffer that is meant for sending arrays to the Voyager
	m_interThreadStorage.appendSerialDataToSend(buffer, size);
}

/***************************************************************************************************************************************************************************/
/********************************************************************-----------------------********************************************************************************/
/********************************************************************|  GETTERS & SETTERS  |********************************************************************************/
/********************************************************************-----------------------********************************************************************************/
/***************************************************************************************************************************************************************************/

void Manager::setEventCallback(std::function<void(Events)> callback)
{
	m_eventsManager.setEventCallback(callback);
}

void Manager::clearEventCallback()
{
	m_eventsManager.clearEventCallback();
}

bool Manager::isVoyagerConnected()
{
	if (m_serialDriverInterface.isConnected() == "") {
		return false;
	}
	else {
		return true;
	}
}

bool Manager::establishConnection()
{
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


	m_serialDriverInterface.close(m_voyagerHandle);
	return false;
}

void Manager::addRawDataBuffer(std::vector<char>* buffer)
{
}

bool Manager::start()
{
	if (!isVoyagerConnected()) {
		std::cerr << "Start called but no Voyager is connected" << std::endl;
		return;
	}
	
	if (!establishConnection()) {
		std::cerr << "Was not able to establish connection" << std::endl;
		return;
	}

	m_threadSerial = std::thread(Manager::serialLoop, 0);


}

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
		updateVoyagerConfiguration();
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
	}
}