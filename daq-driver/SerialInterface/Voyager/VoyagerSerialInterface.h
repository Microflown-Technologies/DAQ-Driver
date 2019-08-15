
#ifdef _VOYAGER
#pragma once
#include "../AbstractDriverInterface.h"

//Standard C++ includes
#include <fstream>
#include <string>
#include <unistd.h> // write(), read(), close()

class VoyagerSerialInterface : public AbstractDriverInterface
{
public:
	VoyagerSerialInterface(EventManager& eventmanager);
	~VoyagerSerialInterface();


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
protected:

private:


};

#endif

