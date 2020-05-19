#include "ClientSocketConnector.h"

ClientSocketConnector::ClientSocketConnector(std::string hostname) : m_hostname(hostname)
{
    ix::initNetSystem();
    startClient();
}

ClientSocketConnector::~ClientSocketConnector()
{

}

void ClientSocketConnector::sendMessage(const std::vector<uint8_t> &message) {
    m_webSocket.sendBinary(std::string(message.begin(), message.end()));
}

size_t ClientSocketConnector::messagesAvailable() {
    return m_messageQueue.size();
}

std::vector<uint8_t> ClientSocketConnector::nextMessage(){
    auto message = m_messageQueue.front();
    m_messageQueue.pop();
    return message;
}

void ClientSocketConnector::stopClient()
{
    m_webSocket.close();
    m_webSocket.stop();
}

void ClientSocketConnector::startClient()
{
    m_webSocket.setUrl("ws://" + m_hostname);
    m_webSocket.setOnMessageCallback(std::bind(&ClientSocketConnector::onMessageCallback, this, std::placeholders::_1));
    m_webSocket.start();
}

void ClientSocketConnector::onMessageCallback(const ix::WebSocketMessagePtr &message) {
    switch(message->type) {
        case ix::WebSocketMessageType::Message:
            m_messageQueue.push(std::vector<uint8_t>(message->str.begin(), message->str.end()));
        break;
        case ix::WebSocketMessageType::Open:
            std::cout << "New connection from client" << std::endl;
        break;
        case ix::WebSocketMessageType::Close:
            std::cout << "Connection was closed" << std::endl;
        break;
        case ix::WebSocketMessageType::Error:
            std::cout << "Connection error" << message->errorInfo.reason << std::endl;
        default:
        break;
    }
}
