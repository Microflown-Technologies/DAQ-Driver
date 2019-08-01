#include "VoyagerAbstractionLayer.h"

void VoyagerAbstractionLayer::compareProtocolBuf()
{
	std::string report;

	m_messageDifferencer.ReportDifferencesToString(&report);
	if (!m_messageDifferencer.Compare(m_voyagerConfiguration, m_voyagerConfiguration)) {
		if (report == "   ") {

		}
		if (report == "   ") {

		}
	};

	//m_voyagerConfiguration.ParseFromArray("");
}

void VoyagerAbstractionLayer::sendNewConfiguration() {
	size_t size = m_voyagerConfiguration.ByteSizeLong();
	char* buffer = new char[size + sizeof(size)];
	buffer[0] = static_cast<char>(size);
	m_voyagerConfiguration.SerializeToArray(buffer + sizeof(size), size);
	m_driverInterface->write(buffer, size + sizeof(size));
	delete[size + sizeof(size)] buffer;
	m_voyagerConfiguration.clear_command();
}

VoyagerAbstractionLayer::VoyagerAbstractionLayer(AbstractDriverInterface* driverInterface) : m_driverInterface(driverInterface)
{
	if(driverInterface->voyagerIsPresent()){
		driverInterface->open();
	}
}

VoyagerAbstractionLayer::~VoyagerAbstractionLayer()
{
	m_voyagerConfiguration.Clear();
}

void VoyagerAbstractionLayer::setSamplingRate(UINT32 samp)
{
	m_voyagerConfiguration.set_samplingfrequency(samp);
	sendNewConfiguration();
}

void VoyagerAbstractionLayer::setGain(Input input, Gain gain)
{
	switch (input) {
	case Flown:
		m_lemo = m_voyagerConfiguration.lemo();
		m_lemo.set_gain(gain);
		m_voyagerConfiguration.set_allocated_lemo(&m_lemo);
		break;
	case Aux1:
		m_bnc1 = m_voyagerConfiguration.bnc1();
		m_bnc1.set_gain(gain);
		m_voyagerConfiguration.set_allocated_bnc1(&m_bnc1);
		break;

	case Aux2:
		m_bnc2 = m_voyagerConfiguration.bnc2();
		m_bnc2.set_gain(gain);
		m_voyagerConfiguration.set_allocated_bnc2(&m_bnc2);
		break;
	}
	sendNewConfiguration();
}

void VoyagerAbstractionLayer::setIepe(Input input, Iepe iepe)
{
	switch (input) {
	case Flown:
		std::cerr << "Cannot set the IEPE of the Lemo connector";
		break;
	case Aux1:
		m_bnc1 = m_voyagerConfiguration.bnc1();
		m_bnc1.set_iepe(iepe);
		m_voyagerConfiguration.set_allocated_bnc1(&m_bnc1);
		break;

	case Aux2:
		m_bnc2 = m_voyagerConfiguration.bnc2();
		m_bnc2.set_iepe(iepe);
		m_voyagerConfiguration.set_allocated_bnc2(&m_bnc2);
		break;
	}
	sendNewConfiguration();
}

void VoyagerAbstractionLayer::setChannelCount(UINT32 channelcount)
{
	m_voyagerConfiguration.set_channelcount(channelcount);
}

void VoyagerAbstractionLayer::setSampleSize(UINT32 samplesize)
{
	m_voyagerConfiguration.set_samplesize(samplesize);
}

void VoyagerAbstractionLayer::setSelectedChannels(UINT32 selectedchannels)
{
	m_voyagerConfiguration.set_selectedchannels(selectedchannels);
}

void VoyagerAbstractionLayer::executeRemoteCommand(const std::string& command) {
	m_voyagerConfiguration.set_command(command);
	sendNewConfiguration();
}

UINT32 VoyagerAbstractionLayer::getSamplingRate()
{
	return m_voyagerConfiguration.samplingfrequency();
}

Gain VoyagerAbstractionLayer::getGain(Input input)
{
	switch (input) {
	case Flown:
		return m_voyagerConfiguration.lemo().gain();
		break;
	case Aux1:
		return m_voyagerConfiguration.bnc1().gain();
		break;

	case Aux2:
		return m_voyagerConfiguration.bnc2().gain();
		break;
	}
}

Iepe VoyagerAbstractionLayer::getIepe(Input input)
{
	switch (input) {
	case Aux1:
		return m_voyagerConfiguration.bnc1().iepe();
		break;
	case Aux2:
		return m_voyagerConfiguration.bnc2().iepe();
		break;
	default:
		std::cerr << "Cannot get the IEPE of the Lemo connector";
		return Iepe::NOTUSED;
	}
}

UINT32 VoyagerAbstractionLayer::getChannelCount()
{
	return m_voyagerConfiguration.channelcount();
}

UINT32 VoyagerAbstractionLayer::getSampleSize()
{
	return m_voyagerConfiguration.samplesize();
}

UINT32 VoyagerAbstractionLayer::getSelectedChannels()
{
	return m_voyagerConfiguration.selectedchannels();
}

UINT64 VoyagerAbstractionLayer::getTimeStamp()
{
	return m_voyagerConfiguration.timestamp();
}


