#include "ProtobufParser.h"

ProtobufParser::ProtobufParser()
{
}

ProtobufConfiguration ProtobufParser::parseSerialData(char *serialData, size_t size)
{

	ProtobufConfiguration ProtoBuffer;
	ProtoBuffer.ParseFromArray(serialData, size);
	return ProtoBuffer;
}

size_t ProtobufParser::serializeProtobufData(char* buffer, ProtobufConfiguration protoBuffer) {

	size_t size = protoBuffer.ByteSizeLong();
	buffer = new char[size];
	protoBuffer.SerializeToArray(buffer, size);
	return size;
}