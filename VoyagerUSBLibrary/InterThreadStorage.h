#ifndef __INTERTHREADSTORAGE_H__
#define __INTERTHREADSTORAGE_H__

#include "EventManager.h"
#include <vector>
#include <mutex>
#include <iostream>

class InterThreadStorage {
public:
	InterThreadStorage();
	virtual ~InterThreadStorage();

	void set_AllowedToRun(bool state);

	const bool allowedToRun();

	void set_Connected(bool state);

	const bool connected();
	
	void addVector(std::string *string);

	std::string* getDataVector();

	bool fillVector(std::string &string, size_t size);

	bool removeVector();

private:

	bool m_allowedToRun;								/*<!A bool that can be shared between threads*/
	std::mutex mutexAllowedToRun;						/*<!Mutex to ensure that m_allowedToRun is not read/written at the same time*/

	bool m_connected;									/*<!A bool that can be shared between threads*/
	std::mutex mutexConnected;							/*<!Mutex to ensure that m_connected is not read/written at the same time*/

	std::vector<char> rawBuffer;						/*<!A vector<char> which can be used to transfer raw serial data between threads*/
	std::mutex mutexRawSerialBuffer;					/*<!Mutex to ensure that rawBuffer is not read/written at the same time*/

	std::vector<char> parsedBuffer;						/*<!A vector<char> which can be used to transfer the parsed audio data between threads*/
	std::mutex mutexParsedBuffer;						/*<!Mutex to ensure that parsedBuffer is not read/written at the same time*/

	std::vector<std::string*> m_emptyBuffers;
	std::mutex m_mutexEmptyBuffers;

	std::vector<std::string*> m_filledBuffers;
	std::mutex m_mutexFilledBuffers;

};

#endif // __INTERTHREADSTORAGE_H__