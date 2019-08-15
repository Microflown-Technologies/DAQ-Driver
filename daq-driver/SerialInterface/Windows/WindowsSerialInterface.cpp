#include "WindowsSerialInterface.h"

WindowsSerialInterface::WindowsSerialInterface(EventManager& eventmanager, InterThreadStorage &interthreadstorage) : AbstractDriverInterface(eventmanager, interthreadstorage)
{
}


WindowsSerialInterface::~WindowsSerialInterface()
{
	
}

std::string WindowsSerialInterface::isConnected(bool loop)
{	
	std::string result;

	do {
		HDEVINFO m_hDevInfo;
		SP_DEVINFO_DATA DeviceInfoData;

		//Refresh the List if currently connected devices
		refreshDevicesListHandle(m_hDevInfo);

		//Loop through all the DeviceList entries
		for (unsigned index = 0; ; index++) {
			DeviceInfoData.cbSize = sizeof(DeviceInfoData);
			if (!SetupDiEnumDeviceInfo(m_hDevInfo, index, &DeviceInfoData)) {
				SetupDiDestroyDeviceInfoList(m_hDevInfo);
				result = "";							// No USB device found with hardware id of the Voyager
				if (m_interThreadStorage.connected) {
					m_interThreadStorage.set_Connected(false);
					m_eventManager.throwHardwareEvent();
				}
			}

			TCHAR HardwareID[1024];
			SetupDiGetDeviceRegistryProperty(m_hDevInfo, &DeviceInfoData, SPDRP_HARDWAREID, NULL, (BYTE*)HardwareID, sizeof(HardwareID), NULL);
			if (_tcsstr(HardwareID, _T("VID_1D6B&PID_0100")) && IsEqualGUID(DeviceInfoData.ClassGuid, GUID_SERENUM_BUS_ENUMERATOR)) {
				std::string ComPort = getComPort(m_hDevInfo, DeviceInfoData);							// Found USB device with hardware id of the Voyager
				SetupDiDestroyDeviceInfoList(m_hDevInfo);
				result = ComPort;
				if (!m_interThreadStorage.connected) {
					m_interThreadStorage.set_Connected(true);
					m_eventManager.throwHardwareEvent();
				}
			}
		}

		SetupDiDestroyDeviceInfoList(m_hDevInfo);
	} while (m_interThreadStorage.allowedToRun() && loop);

	return result;
}

void WindowsSerialInterface::isConnectedLoop()
{
	std::async(std::launch::async, std::bind(&WindowsSerialInterface::isConnected, this, true));
}

VoyagerHandle WindowsSerialInterface::open(std::string input)
{

	HANDLE m_hComm = CreateFile(input.c_str(),		// port name
		GENERIC_READ | GENERIC_WRITE,				// Read/Write
		0,											// No Sharing
		NULL,										// No Security
		OPEN_ALWAYS,								// Open existing port only
		0,											// Non Overlapped I/O
		NULL);										// Null for Comm Devices

	if (m_hComm == INVALID_HANDLE_VALUE)
		printf("Error in opening serial port\n");

	else
		printf("Opening serial port successful\n");

	DCB dcbSerialParams = { 0 };					// Initializing DCB structure
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	if (!GetCommState(m_hComm, &dcbSerialParams))
		printf("Error in setting serial parameters\n");
	else
		printf("Setting serial parameters successful\n");

	dcbSerialParams.BaudRate = CBR_9600;			// Setting BaudRate = 9600
	dcbSerialParams.ByteSize = 8;					// Setting ByteSize = 8
	dcbSerialParams.StopBits = ONESTOPBIT;			// Setting StopBits = 1
	dcbSerialParams.Parity = NOPARITY;				// Setting Parity = None

	if (!SetCommState(m_hComm, &dcbSerialParams))
		printf("Error in setting com state\n");
	else
		printf("Setting com state successful\n");

	COMMTIMEOUTS timeouts = { 0 };
	timeouts.ReadIntervalTimeout = 50;				// in milliseconds
	timeouts.ReadTotalTimeoutConstant = 50;			// in milliseconds
	timeouts.ReadTotalTimeoutMultiplier = 10;		// in milliseconds
	timeouts.WriteTotalTimeoutConstant = 50;		// in milliseconds
	timeouts.WriteTotalTimeoutMultiplier = 10;		// in milliseconds

	if (!SetCommTimeouts(m_hComm, &timeouts))
		printf("Error in setting com timeouts\n");
	else
		printf("Setting com timeouts successful\n");

	if (!SetCommMask(m_hComm, EV_RXCHAR))
		printf("Error in setting com mask\n");
	else
		printf("Setting com mask successful\n");

	clear(m_hComm);
	
	return m_hComm;
}

void WindowsSerialInterface::close(VoyagerHandle handle)
{
	CloseHandle(handle);
}

void WindowsSerialInterface::write(VoyagerHandle handle, const char* data, std::size_t bytes)
{
	if (handle == INVALID_HANDLE_VALUE) {
		std::cerr << "Error in SerialDriverInterface::write - serial port not open" << std::endl;
		return;
	}

	DWORD bytesWritten;
	if (WriteFile(handle, (void*)data, (DWORD)bytes, &bytesWritten, NULL))
	{
		// Flush the bytes
		FlushFileBuffers(handle);
	}
	else
	{
		std::cerr << "Error in SerialDriverINterface::write - writing bytes\n" << std::endl;
	}
}

std::size_t WindowsSerialInterface::read(VoyagerHandle handle, char* data, std::size_t bytes)
{
	DWORD bytesRead;

	if (!ReadFile(handle, data, bytes, &bytesRead, NULL)) {
		std::cerr << "Error in SerialDriverInterface::read - reading bytes\n" << std::endl;
	};
	return std::size_t(bytesRead);
}

void WindowsSerialInterface::dataAvaillableLoop(VoyagerHandle handle, std::mutex& handleMutex)
{
	std::async(std::launch::async, std::bind(&WindowsSerialInterface::bytesAvailable, this, handle, handleMutex, true));
}

std::size_t WindowsSerialInterface::bytesAvailable(VoyagerHandle handle, std::mutex& handleMutex, bool loop)
{
	COMSTAT comStat;
	DWORD errorMask = 0;
	size_t size = 0;

	do {
		if (!ClearCommError(handle, &errorMask, &comStat)) {
			return 0;
		}
		if (size = comStat.cbInQue) {
			m_eventManager.throwDataEvent();
		}
	} while (m_interThreadStorage.allowedToRun() && loop);


	return size;
}

void WindowsSerialInterface::clear(VoyagerHandle handle)
{
	if (!PurgeComm(handle, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT)) {
		std::cerr << "Error in SerialDriverInterface::clear - not able to clear buffer" << std::endl;
		std::cerr <<  GetLastError() << std::endl;
	}
}

void WindowsSerialInterface::refreshDevicesListHandle(HDEVINFO &m_hDevInfo)
{
	//SetupDiDestroyDeviceInfoList(m_hDevInfo);
	m_hDevInfo = SetupDiGetClassDevs(NULL, TEXT("USB"), NULL, DIGCF_PRESENT | DIGCF_ALLCLASSES);
}

std::string WindowsSerialInterface::getComPort(HDEVINFO m_hDevInfo, SP_DEVINFO_DATA DeviceInfoData)
{
	TCHAR FriendlyName[1024];
	SetupDiGetDeviceRegistryProperty(m_hDevInfo, &DeviceInfoData, SPDRP_FRIENDLYNAME, NULL, (BYTE*)FriendlyName, sizeof(FriendlyName), NULL);
	return(std::string("\\\\.\\COM" + std::string(FriendlyName).substr(22, std::string(FriendlyName).find(')') - 22)));
}
