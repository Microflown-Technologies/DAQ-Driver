#include "SerialDriverInterface.h"

SerialDriverInterface::SerialDriverInterface()
{
	m_hDevInfo = SetupDiGetClassDevs(NULL, TEXT("USB"), NULL, DIGCF_PRESENT | DIGCF_ALLCLASSES);
}


SerialDriverInterface::~SerialDriverInterface()
{
	SetupDiDestroyDeviceInfoList(m_hDevInfo);
}

bool SerialDriverInterface::voyagerIsPresent()
{	
	refreshDevicesListHandle();
	for (unsigned index = 0; ; index++) {
		DeviceInfoData.cbSize = sizeof(DeviceInfoData);
		if (!SetupDiEnumDeviceInfo(m_hDevInfo, index, &DeviceInfoData)) {
			return false;							// No USB device found with hardware id of the Voyager
		}

		TCHAR HardwareID[1024];
		SetupDiGetDeviceRegistryProperty(m_hDevInfo, &DeviceInfoData, SPDRP_HARDWAREID, NULL, (BYTE*)HardwareID, sizeof(HardwareID), NULL);
		if (_tcsstr(HardwareID, _T("VID_1D6B&PID_0100")) && IsEqualGUID(DeviceInfoData.ClassGuid, GUID_SERENUM_BUS_ENUMERATOR)) {
			return true;							// Found USB device with hardware id of the Voyager
		}
	}
	return false;
}

void SerialDriverInterface::open()
{

	m_hComm = CreateFile(getComPort().c_str(),		// port name
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
}

void SerialDriverInterface::close()
{
	CloseHandle(m_hComm);
}

void SerialDriverInterface::write(const char* data, std::size_t bytes)
{
	if (m_hComm == INVALID_HANDLE_VALUE) {
		std::cerr << "Error in SerialDriverInterface::write - serial port not open" << std::endl;
		return;
	}

	DWORD bytesWritten;
	if (WriteFile(m_hComm, (void*)data, (DWORD)bytes, &bytesWritten, NULL))
	{
		// Flush the bytes
		FlushFileBuffers(m_hComm);
	}
	else
	{
		std::cerr << "Error in SerialDriverINterface::write - writing bytes\n" << std::endl;
	}
}

std::size_t SerialDriverInterface::read(char* data, std::size_t bytes)
{
	DWORD bytesRead;

	if (!ReadFile(m_hComm, data, bytes, &bytesRead, NULL)) {
		std::cerr << "Error in SerialDriverInterface::read - reading bytes\n" << std::endl;
	};

	return std::size_t(bytesRead);
}

std::size_t SerialDriverInterface::bytesAvailable()
{
	COMSTAT comStat;
	DWORD errorMask = 0;

	ClearCommError(m_hComm, &errorMask, &comStat);
	if (comStat.cbInQue == 3435973836) {
		return 0;
	}
	return std::size_t(comStat.cbInQue);
}

void SerialDriverInterface::clear()
{
	if (!PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT)) {
		std::cerr << "Error in SerialDriverInterface::clear - not able to clear buffer" << std::endl;
		std::cerr <<  GetLastError() << std::endl;
	
	}
}

void SerialDriverInterface::refreshDevicesListHandle()
{
	SetupDiDestroyDeviceInfoList(m_hDevInfo);
	m_hDevInfo = SetupDiGetClassDevs(NULL, TEXT("USB"), NULL, DIGCF_PRESENT | DIGCF_ALLCLASSES);
}

std::string SerialDriverInterface::getComPort()
{
	TCHAR FriendlyName[1024];
	SetupDiGetDeviceRegistryProperty(m_hDevInfo, &DeviceInfoData, SPDRP_FRIENDLYNAME, NULL, (BYTE*)FriendlyName, sizeof(FriendlyName), NULL);
	return(std::string("\\\\.\\COM" + std::string(FriendlyName).substr(22, std::string(FriendlyName).find(')') - 22)));
}
