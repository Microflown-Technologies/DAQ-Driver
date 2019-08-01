#define NOMINMAX
#include "SerialDriverInterface.h"

#pragma warning( push )
#pragma warning( disable : 4251)
#pragma warning( disable : 6495)
#pragma warning( disable : 26495)
#include "Configuration.pb.h"
#include <google/protobuf/util/message_differencer.h>
#pragma warning( pop )

enum Input { Aux1, Aux2, Flown };

#pragma once
class VoyagerAbstractionLayer
{
	
public:

	VoyagerAbstractionLayer(AbstractDriverInterface *driverInterface);
	~VoyagerAbstractionLayer();

	/**
	* @details
	* @params
	**/
	void setSamplingRate(UINT32 samp);

	/**
	* @details
	* @params
	* @params
	**/
	void setGain(Input input, Gain gain);

	/**
	* @details
	* @params
	* @params
	**/
	void setIepe(Input input, Iepe iepe);

	/**
	* @details
	* @params
	* @params
	**/
	void setChannelCount(UINT32 channelcount);

	/**
	* @details
	* @params
	* @params
	**/
	void setSampleSize(UINT32 samplesize);

	/**
	* @details
	* @params
	* @params
	**/
	void setSelectedChannels(UINT32 selectedchannels);

	/**
	* @details
	* @params
	**/
	void executeRemoteCommand(const std::string &command);

	/**
	* @details
	* @returns
	**/
	UINT32 getSamplingRate();

	/**
	* @details
	* @params
	* @returns
	**/
	Gain getGain(Input source);

	/**
	* @details
	* @params 
	* @returns
	**/
	Iepe getIepe(Input source);

	/**
	* @details
	* @params
	* @returns
	**/
	UINT32 getChannelCount();

	/**
	* @details
	* @params
	* @returns
	**/
	UINT32 getSampleSize();

	/**
	* @details
	* @params
	* @returns
	**/
	UINT32 getSelectedChannels();

	/**
	* @details
	* @params
	* @returns
	**/
	UINT64 getTimeStamp();

protected:
	/**
	* @details compares two protocols buffers with each other
	**/
	void compareProtocolBuf();

	/**
	* @details Sends the new configuration to the Voyager
	**/
	void sendNewConfiguration();

private:

	BNC m_bnc1;
	BNC m_bnc2;
	Lemo m_lemo;
	AbstractDriverInterface *m_driverInterface;
	VoyagerConfiguration m_voyagerConfiguration;
	VoyagerConfiguration m_receivedConfiguration;
	google::protobuf::util::MessageDifferencer m_messageDifferencer;
};