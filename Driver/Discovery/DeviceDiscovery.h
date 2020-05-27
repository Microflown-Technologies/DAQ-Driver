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
    DeviceDiscovery();

    /**
     * @brief discover Discovers all the DAQ devices on the network
     */
    static std::vector<pDiscoveredDevice> discover();

    static std::map<std::string, std::string> parseTxtRecord(const std::string &record);

    static std::pair<std::string,std::string> splitTxtRecord(const std::string &record);

    static std::string getIpForResponce(Zeroconf::mdns_responce responce);

    static uint16_t getPortForResponce(Zeroconf::mdns_responce responce);


    static void *get_in_addr(sockaddr_storage* sa);

protected:

    /**
     * @brief initialize Intializes the DeviceDiscovery (Networking etc.)
     */
    static void initialize();
};

#endif // DEVICEDISCOVERY_H
