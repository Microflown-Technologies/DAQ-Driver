#pragma warning( push )
#pragma warning( disable : 4251)
#pragma warning( disable : 6495)
#pragma warning( disable : 26495)
#include "ProtobufConfiguration.pb.h"
#include <google/protobuf/util/message_differencer.h>
#pragma warning( pop )

#pragma once
class ProtobufComparer
{
public:

	ProtobufComparer();
	~ProtobufComparer();

	bool compareProtobufs(ProtobufConfiguration &proto1, ProtobufConfiguration &proto2);

	void addIgnoreField(std::string Field);


protected:

private:

	google::protobuf::util::MessageDifferencer m_messageDifferencer;
	google::protobuf::util::MessageDifferencer::Reporter *m_differenceReporter;
	ProtobufConfiguration m_savedProtobuf;
};

