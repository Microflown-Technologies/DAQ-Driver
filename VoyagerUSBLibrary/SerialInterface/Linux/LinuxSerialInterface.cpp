#include "LinuxSerialInterface.h"

#ifdef __linux__
LinuxSerialInterface::LinuxSerialInterface(EventManager& eventmanager) : AbstractDriverInterface(eventmanager)
{
}

LinuxSerialInterface::~LinuxSerialInterface()
{
}

std::string LinuxSerialInterface::isConnected()
{
	return std::string();
}

VoyagerHandle LinuxSerialInterface::open(std::string port)
{
	return VoyagerHandle();
}

void LinuxSerialInterface::close(VoyagerHandle handle)
{
}

void LinuxSerialInterface::write(VoyagerHandle handle, const char* data, std::size_t bytes)
{
}

std::size_t LinuxSerialInterface::read(VoyagerHandle handle, char* data, std::size_t bytes)
{
	return std::size_t();
}

std::size_t LinuxSerialInterface::bytesAvailable(VoyagerHandle handle)
{
	return std::size_t();
}

void LinuxSerialInterface::clear(VoyagerHandle handle)
{
}

#endif
