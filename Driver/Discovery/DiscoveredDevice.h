#ifndef DISCOVEREDDEVICE_H
#define DISCOVEREDDEVICE_H

//STD framework
#include <string>
#include <memory>

class DiscoveredDevice
{
public:
    DiscoveredDevice();

    std::string ipAddress() const;
    void setIpAddress(const std::string &ipAddress);

    std::string driverVersion() const;
    void setDriverVersion(const std::string &driverVersion);

    std::string deviceName() const;
    void setDeviceName(const std::string &deviceName);

    std::string deviceSerial() const;
    void setDeviceSerial(const std::string &deviceSerial);

    std::string driverName() const;
    void setDriverName(const std::string &driverName);

    std::string modelName() const;
    void setModelName(const std::string &modelName);

    std::string softwareVersion() const;
    void setSoftwareVersion(const std::string &softwareVersion);

    uint16_t port() const;
    void setPort(const uint16_t &port);

private:
    uint16_t m_port;
    std::string m_ipAddress;
    std::string m_deviceName;
    std::string m_deviceSerial;
    std::string m_driverName;
    std::string m_driverVersion;
    std::string m_modelName;
    std::string m_softwareVersion;


};
typedef std::shared_ptr<DiscoveredDevice> pDiscoveredDevice;

#endif // DISCOVEREDDEVICE_H
