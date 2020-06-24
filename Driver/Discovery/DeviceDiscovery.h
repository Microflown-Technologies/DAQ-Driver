#ifndef DEVICEDISCOVERY_H
#define DEVICEDISCOVERY_H

//STD framework
#include <map>
#include <regex>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

//OS Networking
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>
#include <windows.h>
#include <SetupAPI.h>
#pragma once
#pragma comment (lib, "Setupapi.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

//Zeroconf
#include "zeroconf.hpp"

//Internal headers
#include "DiscoveredDevice.h"
#include "SocketConnector/AbstractSocketConnector.h"

/**
 * @brief The DeviceDiscovery class can be used to discovery DAQ devices
 */
class DeviceDiscovery
{
public:
    /**
     * @brief discover Discovers all the DAQ devices on the network
     * @param scanTime amount of seconds to scan (default 1 sec, maybe longer is needed on large networks)
     * @param localPresent if true will also return dev
     */
    static std::vector<pDiscoveredDevice> discover(std::time_t scanTime = 1, bool localPresent = false);

    /**
     * @brief presentVoyagers List of Voyagers that are connected through usb
     * @return list of Voyagers that are connected through usb
     */
    static std::vector<pDiscoveredDevice> presentVoyagers();

protected:
    /**
     * @brief parseTxtRecord Parses the txt records
     * @param record raw text record to parse
     * @return map of txt records key and value
     */
    static std::map<std::string, std::string> parseTxtRecord(const std::string &record);

    static std::pair<std::string,std::string> splitTxtRecord(const std::string &record);

    static std::string getIpForResponce(Zeroconf::mdns_responce responce);

    static uint16_t getPortForResponce(Zeroconf::mdns_responce responce);

    static void *get_in_addr(sockaddr_storage* sa);

    /**
     * @brief initialize Intializes the DeviceDiscovery (Networking etc.)
     */
    static void initialize();

    /**
     * @brief deinitialize Deintializes/cleans up the DeviceDiscovery (Networking etc.)
     */
    static void deinitialize();

    static void refreshDevicesListHandle(HDEVINFO &m_hDevInfo);

private:
    static HANDLE m_serialHandle;
    static DCB m_serialPortParameters;
};

#endif // DEVICEDISCOVERY_H
