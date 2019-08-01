#pragma once

//STD framework
#include <iostream>
#include <functional>

class AbstractDriverInterface
{
public:
	AbstractDriverInterface();

	/**
	* @details States the connection between Voyager and system is able to be
	**/
	enum ConnectionState {
		Disconnected,
		Initializing,
		Disconnecting,
		Connected
	};

	/**
	* @details Check if a Voyager is connected to the system
	* @returns returns true if a Voyager is connected
	**/
	virtual bool voyagerIsPresent() = 0;

	/**
	* @details Attempts to open a connection to the Voyager
	**/
	virtual void open() = 0;

	/**
	* @details Attempts to close the connection to the Voyager (if any exists)
	**/
	virtual void close() = 0;

	/**
	* @details gets the current connection state of the connection between the voyager and the system
	* @returns the current state of the connection between the Voyager and the system
	**/
	ConnectionState connectionState();

	/**
	* @details Set a callback function that is called when a Voyager is connected to the system
	* @param callback function pointer to callback
	**/
	void setVoyagerConnectedCallback(std::function<void(void)> callback);

	/**
	* @details setConnectionStateChangeCallback Sets a callback function that is called when the state of connection between 
	* the Voyager and the PC changes
	* @param callback function pointer to callback
	**/
	void setConnectionStateChangeCallback(std::function<void(ConnectionState)> callback);

	/**
	* @details Sets a callback function that is called when new data is available
	* @param function pointer to callback
	**/
	void setNewDataAvailableCallback(std::function<void(void)> callback);

	/**
	* @details Writes/sends data to the Voyager
	* @param data pointer to data to write
	* @param bytes amount of bytes to write from data pointer
	**/
	virtual void write(const char* data, std::size_t bytes) = 0;

	/**
	* @details Returns the amount of bytes that are available to read 
	* @returns the amount of bytes that are available for reading
	**/
	virtual std::size_t bytesAvailable() = 0;

	/**
	* @details Copies available data to data pointer
	* @param data location to write available data to
	* @param bytes amount of bytes to write
	* @warning if bytes is larger than bytesAvailable() the remaining space will be filled with zeroes
	* @returns the amount of actual bytes that where copied to data
	**/
	virtual std::size_t read(char* data, std::size_t bytes) = 0;

protected:
	/**
	* @details check if m_voyagerConnectedCallback has been set and calls the callback if that is true
	**/
	void callVoyagerConnectedCallback();

	/**
	* @details check if m_newDataAvailableCallback has been set and calls the callback if that is true
	**/
	void callNewDataAvailableCallback();

	/**
	* @details check if m_connectionStateChangedCallback has been set and calls the callback if that is true
	* @param connectionState current connection state
	**/
	void callConnectionStateChangedCallback(ConnectionState connectionState);

private:
	ConnectionState m_connectionState; /*! Contains the current state of the connection*/
	std::function<void(void)> m_voyagerConnectedCallback; /*!< Is called when a Voyager is connected to the system*/
	std::function<void(void)> m_newDataAvailableCallback; /*!< Is called when new data is recieved from the Voyager */
	std::function<void(ConnectionState)> m_connectionStateChangedCallback; /*!< Is called when the state of connection between Voyager and the system changed */
};

