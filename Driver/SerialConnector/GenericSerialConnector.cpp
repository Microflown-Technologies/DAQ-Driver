#ifdef _WIN32
#include "GenericSerialConnector.h"
GenericSerialConnector::GenericSerialConnector() : m_serialHandle(INVALID_HANDLE_VALUE)
{

}

void GenericSerialConnector::write(const std::vector<uint8_t> &data) {
    WriteFile(m_serialHandle, data.data(), data.size(), nullptr, nullptr);
}

void GenericSerialConnector::priorityWrite(const std::vector<uint8_t> &data)
{
    write(data);
    FlushFileBuffers(m_serialHandle);
}

size_t GenericSerialConnector::dataAvailable() const {
    DWORD errorMask = 0;
    COMSTAT serialPortStatistics;
    if(ClearCommError(m_serialHandle, &errorMask, &serialPortStatistics) == 0) return 0;
    return serialPortStatistics.cbInQue;
}

std::vector<uint8_t> GenericSerialConnector::read()
{
    if(m_serialHandle == INVALID_HANDLE_VALUE) return std::vector<uint8_t>();
    //Allocate space in memory to read to
    std::vector<uint8_t> dataBuffer;
    dataBuffer.resize(dataAvailable());
    DWORD bytesRead;
    //Read data to buffer
    ReadFile(m_serialHandle, dataBuffer.data(), dataBuffer.size(), &bytesRead, nullptr);
    dataBuffer.resize(bytesRead);
    return dataBuffer;
}

bool GenericSerialConnector::open()
{
    //Check if Voyager is connected
    if (!voyagerConnected()) return false;
    //Open the serial port
    m_serialHandle = CreateFile(getVoyagerComPort().c_str(),
                                 GENERIC_READ | GENERIC_WRITE,
                                 0,
                                 nullptr,
                                 OPEN_EXISTING,
                                 FILE_ATTRIBUTE_NORMAL,
                                 nullptr);
    if(m_serialHandle == INVALID_HANDLE_VALUE) return false;
    //Configure the serial port
    m_serialPortParameters.DCBlength = sizeof (m_serialPortParameters);
    if(!GetCommState(m_serialHandle, &m_serialPortParameters)) return false;
    m_serialPortParameters.BaudRate = 2000000;
    m_serialPortParameters.ByteSize = 8;
    m_serialPortParameters.StopBits = ONESTOPBIT;
    m_serialPortParameters.Parity   = NOPARITY;
    if(!SetCommState(m_serialHandle, &m_serialPortParameters)) return false;
    //Set serial port timeout
    if(!GetCommTimeouts(m_serialHandle, &m_serialPortTimeout)) return false;
    m_serialPortTimeout.ReadIntervalTimeout = 1;				// in milliseconds
    m_serialPortTimeout.ReadTotalTimeoutConstant = 5;			// in milliseconds
    m_serialPortTimeout.ReadTotalTimeoutMultiplier = 1;		// in milliseconds
    m_serialPortTimeout.WriteTotalTimeoutConstant = 1;         // in milliseconds
    m_serialPortTimeout.WriteTotalTimeoutMultiplier = 1;		// in milliseconds
    if(!SetCommTimeouts(m_serialHandle, &m_serialPortTimeout)) return false;
    if (!PurgeComm(m_serialHandle, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT)) return false;
    return true;
}

void GenericSerialConnector::close()
{
    CloseHandle(m_serialHandle);
    m_serialHandle = INVALID_HANDLE_VALUE;
}

bool GenericSerialConnector::isOpen() {
    return (m_serialHandle != INVALID_HANDLE_VALUE);
}

bool GenericSerialConnector::voyagerConnected() {
    return !getVoyagerComPort().empty();
}

std::string GenericSerialConnector::getVoyagerComPort()
{
    HDEVINFO m_hDevInfo;
    SP_DEVINFO_DATA DeviceInfoData;

    //Refresh the List if currently connected devices
    refreshDevicesListHandle(m_hDevInfo);

    //Loop through all the DeviceList entries
    for (unsigned index = 0; ; index++) {
        DeviceInfoData.cbSize = sizeof(DeviceInfoData);
        if (!SetupDiEnumDeviceInfo(m_hDevInfo, index, &DeviceInfoData)) {
            SetupDiDestroyDeviceInfoList(m_hDevInfo);
                return ""; 							// No USB device found with hardware id of the Voyager
        }

        TCHAR HardwareID[1024];
        SetupDiGetDeviceRegistryProperty(m_hDevInfo, &DeviceInfoData, SPDRP_HARDWAREID, nullptr, reinterpret_cast<BYTE*>(HardwareID), sizeof(HardwareID), nullptr);
        if (_tcsstr(HardwareID, _T("VID_1D6B&PID_0100")) && IsEqualGUID(DeviceInfoData.ClassGuid, GUID_SERENUM_BUS_ENUMERATOR)) {
            std::string ComPort = getComPort(m_hDevInfo, DeviceInfoData);							// Found USB device with hardware id of the Voyager
            SetupDiDestroyDeviceInfoList(m_hDevInfo);
            return ComPort;
        }
    }
}

void GenericSerialConnector::refreshDevicesListHandle(HDEVINFO &m_hDevInfo) {
    m_hDevInfo = SetupDiGetClassDevs(nullptr, TEXT("USB"), nullptr, DIGCF_PRESENT | DIGCF_ALLCLASSES);
}

std::string GenericSerialConnector::getComPort(HDEVINFO m_hDevInfo, SP_DEVINFO_DATA DeviceInfoData) {
    TCHAR FriendlyName[1024];
    SetupDiGetDeviceRegistryProperty(m_hDevInfo, &DeviceInfoData, SPDRP_FRIENDLYNAME, nullptr, reinterpret_cast<BYTE*>(FriendlyName), sizeof(FriendlyName), nullptr);
    return(std::string("COM" + std::string(FriendlyName).substr(22, std::string(FriendlyName).find(')') - 22)));
}
#endif
