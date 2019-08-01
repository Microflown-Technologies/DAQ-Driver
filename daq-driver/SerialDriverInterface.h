#pragma once
#include "AbstractDriverInterface.h"
#include <windows.h>
#include <SetupAPI.h>
#include <tchar.h>

#pragma once
#pragma comment (lib, "Setupapi.lib")

class SerialDriverInterface : public AbstractDriverInterface
{
public:
	SerialDriverInterface();
	~SerialDriverInterface();

	/**
	* @details Check if a Voyager is connected to the system
	* @returns returns true if a Voyager is connected
	**/
	bool voyagerIsPresent() override;

	/**
	* @details Attempts to open a connection to the Voyager
	**/
	void open() override;

	/**
	* @details Attempts to close the connection to the Voyager (if any exists)
	**/
	void close() override;

	/**
	* @details Writes/sends data to the Voyager
	* @param data pointer to data to write
	* @param bytes amount of bytes to write from data pointer
	**/
	void write(const char* data, std::size_t bytes) override;

	/**
	* @details Copies available data to data pointer
	* @param data location to write available data to
	* @param bytes amount of bytes to write
	* @warning if bytes is larger than bytesAvailable() the remaining space will be filled with zeroes
	* @returns the amount of actual bytes that where copied to data
	**/
	std::size_t read(char* data, std::size_t bytes)	override;
	
	/**
	* @details Returns the amount of bytes that are available to read
	* @returns the amount of bytes that are available for reading
	**/
	std::size_t bytesAvailable() override;

	/**
	* @details Clears the pending buffer of the serial port 
	**/
	void clear();

	/**
	* @details Refreshes the list of connected USB devices
	**/
	void refreshDevicesListHandle();

private:

	/**
	* @details
	**/
	std::string getComPort();

	HANDLE m_hComm;
	HDEVINFO m_hDevInfo = NULL;
	SP_DEVINFO_DATA DeviceInfoData = SP_DEVINFO_DATA();
	std::string m_portNumber;
};

