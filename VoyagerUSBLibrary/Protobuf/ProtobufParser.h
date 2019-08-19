#define NOMINMAX


#pragma warning( push )
#pragma warning( disable : 4251)
#pragma warning( disable : 6495)
#pragma warning( disable : 26495)
#include "ProtobufConfiguration.pb.h"
#include <google/protobuf/util/message_differencer.h>
#pragma warning( pop )

class ProtobufParser
{
public:
	ProtobufParser();

	/**
	*@details Converts a char array into a Protobuffer
	*@param char array that contains a Protobuffer
	*@param size_t of send char * array
	*@return ProtobufConfiguration of the char array
	**/
	ProtobufConfiguration parseSerialData(char* serialData, size_t size);

	/**
	*@details
	*@param buffer char* array that is used to store the serialized Protobuf
	*@param protoBuffer that is used to convert to a char* array
	*@return size of the created char* array
	**/
	size_t serializeProtobufData(char* buffer, ProtobufConfiguration protoBuffer);

private:

};

