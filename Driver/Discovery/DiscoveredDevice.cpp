#include "DiscoveredDevice.h"

DiscoveredDevice::DiscoveredDevice()
{

}

std::string DiscoveredDevice::ipAddress() const
{
    return m_ipAddress;
}

void DiscoveredDevice::setIpAddress(const std::string &ipAddress)
{
    m_ipAddress = ipAddress;
}

std::string DiscoveredDevice::driverVersion() const
{
    return m_driverVersion;
}

void DiscoveredDevice::setDriverVersion(const std::string &driverVersion)
{
    m_driverVersion = driverVersion;
}

std::string DiscoveredDevice::deviceName() const
{
    return m_deviceName;
}

void DiscoveredDevice::setDeviceName(const std::string &deviceName)
{
    m_deviceName = deviceName;
}

std::string DiscoveredDevice::deviceSerial() const
{
    return m_deviceSerial;
}

void DiscoveredDevice::setDeviceSerial(const std::string &deviceSerial)
{
    m_deviceSerial = deviceSerial;
}

std::string DiscoveredDevice::driverName() const
{
    return m_driverName;
}

void DiscoveredDevice::setDriverName(const std::string &driverName)
{
    m_driverName = driverName;
}

std::string DiscoveredDevice::modelName() const
{
    return m_modelName;
}

void DiscoveredDevice::setModelName(const std::string &modelName)
{
    m_modelName = modelName;
}

std::string DiscoveredDevice::softwareVersion() const
{
    return m_softwareVersion;
}

void DiscoveredDevice::setSoftwareVersion(const std::string &softwareVersion)
{
    m_softwareVersion = softwareVersion;
}

uint16_t DiscoveredDevice::port() const
{
    return m_port;
}

void DiscoveredDevice::setPort(const uint16_t &port)
{
    m_port = port;
}
