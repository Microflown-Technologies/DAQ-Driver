#include "VoyagerSerialInterface.h"

VoyagerSerialInterface::VoyagerSerialInterface(EventManager& eventmanager) : AbstractDriverInterface(eventmanager)
{

}

VoyagerSerialInterface::~VoyagerSerialInterface()
{
}

std::string VoyagerSerialInterface::isConnected()
{
	std::ifstream usbGadgetStateFile;
	std::string gadgetStateLine;
	usbGadgetStateFile.open("/sys/class/udc/ci_hdrc.0/state");
	std::getline(usbGadgetStateFile, gadgetStateLine);	

	if (gadgetStateLine == "configured") {
		return "/dev/ttyACM0";
	}
	
	return "";
}

VoyagerHandle VoyagerSerialInterface::open(std::string port)
{
	return VoyagerHandle();
}

void VoyagerSerialInterface::close(VoyagerHandle handle)
{
}

void VoyagerSerialInterface::write(VoyagerHandle handle, const char* data, std::size_t bytes)
{
}

std::size_t VoyagerSerialInterface::read(VoyagerHandle handle, char* data, std::size_t bytes)
{
	return std::size_t();
}

std::size_t VoyagerSerialInterface::bytesAvailable(VoyagerHandle handle)
{
	return std::size_t();
}

void VoyagerSerialInterface::clear(VoyagerHandle handle)
{
}
