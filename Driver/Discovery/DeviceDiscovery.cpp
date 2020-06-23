#include "DeviceDiscovery.h"

std::vector<pDiscoveredDevice> DeviceDiscovery::discover(time_t scanTime)
{
   initialize();
   std::vector<pDiscoveredDevice> discoveredDevices = presentVoyagers();
    //Prepare DNS query
   static const std::string MdnsQuery = "_voyager._tcp.local";
   //Execute query and store results
   std::vector<Zeroconf::mdns_responce> responces;
   Zeroconf::Resolve(MdnsQuery, scanTime, &responces);
   for(Zeroconf::mdns_responce responce: responces) {
       for(Zeroconf::Detail::mdns_record record: responce.records) {
           if(record.type == 16) {
               char* dataPtr = reinterpret_cast<char*>(&responce.data[record.pos]);
               std::string txtRecord;
               txtRecord.resize(record.len);
               txtRecord.assign(dataPtr, record.len);
               auto parsedTxtRecord = parseTxtRecord(txtRecord);
               //Add discovered device
               pDiscoveredDevice discoveredDevice(new DiscoveredDevice());
               discoveredDevice->setPort(8080);
               discoveredDevice->setIpAddress(getIpForResponce(responce));
               discoveredDevice->setDeviceName(parsedTxtRecord["DeviceName"]);
               discoveredDevice->setDeviceSerial(parsedTxtRecord["DeviceSerial"]);
               discoveredDevice->setDriverName(parsedTxtRecord["DriverName"]);
               discoveredDevice->setDriverVersion(parsedTxtRecord["DriverVersion"]);
               discoveredDevice->setModelName(parsedTxtRecord["ModelName"]);
               discoveredDevice->setSoftwareVersion(parsedTxtRecord["SoftwareVersion"]);
               discoveredDevices.push_back(discoveredDevice);
           }
       }
   }
   deinitialize();
   return discoveredDevices;
}

std::vector<pDiscoveredDevice> DeviceDiscovery::presentVoyagers()
{
    std::vector<pDiscoveredDevice> presentVoyagers;
    std::vector<libusbp::device> connectedDevices = libusbp::list_connected_devices();
    for(libusbp::device device : connectedDevices) {
        if(device.get_vendor_id() == 0x1d6b && device.get_product_id() == 0x0100) {
            pDiscoveredDevice discoveredDevice(new DiscoveredDevice());
            discoveredDevice->setPort(8080);
            discoveredDevice->setIpAddress("172.20.0.1");
            discoveredDevice->setDeviceName("Voyager");
            presentVoyagers.push_back(discoveredDevice);
        }
    }
    return presentVoyagers;
}

std::map<std::string, std::string> DeviceDiscovery::parseTxtRecord(const std::string &record)
{
    //Copy string and create map
    std::string recordCopy = record;
    std::map<std::string, std::string> parsedRecords;
    //Prepare regex
    std::regex regex("[a-zA-Z0-9 .-]*=[a-zA-Z0-9 .-]*");
    //Split string by regex
    std::smatch match;
    while (std::regex_search(recordCopy, match, regex)) {
        std::string parsedRecord = match[0];
        parsedRecords.insert(splitTxtRecord(parsedRecord));
        recordCopy = match.suffix();
    }

    return parsedRecords;
}

std::pair<std::string, std::string> DeviceDiscovery::splitTxtRecord(const std::string &record)
{
    std::string segment;
    std::stringstream recordStringStream(record);
    std::vector<std::string> segmentList;
    while(std::getline(recordStringStream, segment, '=')) {
       segmentList.push_back(segment);
    }
    if(segmentList.size() >= 2) {
        return {segmentList[0], segmentList[1]};
    } else return {};
}

std::string DeviceDiscovery::getIpForResponce(Zeroconf::mdns_responce responce) {
    char buffer[INET6_ADDRSTRLEN + 1] = {0};
    inet_ntop(responce.peer.ss_family, get_in_addr(&responce.peer), buffer, INET6_ADDRSTRLEN);
    return  buffer;
}

uint16_t DeviceDiscovery::getPortForResponce(Zeroconf::mdns_responce responce) {
    if (responce.peer.ss_family == AF_INET)  return reinterpret_cast<sockaddr_in*>(get_in_addr(&responce.peer))->sin_port;
    if (responce.peer.ss_family == AF_INET6)  return reinterpret_cast<sockaddr_in6*>(get_in_addr(&responce.peer))->sin6_port;
    return AbstractSocketConnector::defaultPort;
}

void *DeviceDiscovery::get_in_addr(sockaddr_storage *sa) {
    if (sa->ss_family == AF_INET)
        return &reinterpret_cast<sockaddr_in*>(sa)->sin_addr;

    if (sa->ss_family == AF_INET6)
        return &reinterpret_cast<sockaddr_in6*>(sa)->sin6_addr;

    return nullptr;
}

void DeviceDiscovery::initialize()
{
#ifdef WIN32
    WSADATA wsa;
    if (WSAStartup(0x202, &wsa) != 0)
    {
        std::cout << "Unable to initialize WinSock" << std::endl;
    }
#endif
}

void DeviceDiscovery::deinitialize()
{
#ifdef WIN32
    WSACleanup();
#endif
}
