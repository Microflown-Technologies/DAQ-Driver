#include "GenericSerialConnector.h"

GenericSerialConnector::GenericSerialConnector()
{

}

void GenericSerialConnector::write(const std::vector<uint8_t> &data)
{

}

void GenericSerialConnector::priorityWrite(const std::vector<uint8_t> &data)
{

}

uint64_t GenericSerialConnector::dataAvailable() const
{
    return 0;
}

std::vector<uint8_t> GenericSerialConnector::read()
{

}

bool GenericSerialConnector::voyagerConnected()
{
    int numberOfComPorts = comEnumerate();
    for(int currentCom = 0; numberOfComPorts > currentCom; currentCom++) {
        std::cout << comGetPortName(currentCom) << std::endl;
    }
}
