#define NOMINMAX
#include "../../EventsManager.h"
#include "../AbstractDriverInterface.h"
#include <windows.h>
#include <SetupAPI.h>
#include <tchar.h>
#include <thread>

#pragma once
#pragma comment (lib, "Setupapi.lib")

class WindowsSerialInterface : public AbstractDriverInterface
{
public:
	WindowsSerialInterface();
	~WindowsSerialInterface();

	/**
	* @details Check if a Voyager is connected to the system
	* @returns returns true if a Voyager is connected
	**/
	std::string isConnected() override;

	/**
	* @details Attempts to open a connection to the Voyager
	**/
	VoyagerHandle open(std::string port) override;

	/**
	* @details Attempts to close the connection to the Voyager (if any exists)
	**/
	void close(VoyagerHandle handle) override;

	/**
	* @details Writes/sends data to the Voyager
	* @param data pointer to data to write
	* @param bytes amount of bytes to write from data pointer
	**/
	void write(VoyagerHandle handle, const char* data, std::size_t bytes) override;

	/**
	* @details Copies available data to data pointer
	* @param data location to write available data to
	* @param bytes amount of bytes to write
	* @warning if bytes is larger than bytesAvailable() the remaining space will be filled with zeroes
	* @returns the amount of actual bytes that where copied to data
	**/
	std::size_t read(VoyagerHandle handle, char* data, std::size_t bytes)	override;
	
	/**
	* @details Returns the amount of bytes that are available to read
	* @returns the amount of bytes that are available for reading
	**/
	std::size_t bytesAvailable(VoyagerHandle handle) override;

	/**
	* @details Clears the pending buffer of the serial port 
	**/
	void clear(VoyagerHandle handle) override;

private:

	/**
	* @details Refreshes the list of connected USB devices
	**/
	void refreshDevicesListHandle(HDEVINFO &m_hDevInfo);

	/**
	* @details
	**/
	std::string getComPort(HDEVINFO m_hDevInfo, SP_DEVINFO_DATA DeviceInfoData);
};

