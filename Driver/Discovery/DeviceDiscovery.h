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
     */
    static std::vector<pDiscoveredDevice> discover(std::time_t scanTime = 1);

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
};

#endif // DEVICEDISCOVERY_H
