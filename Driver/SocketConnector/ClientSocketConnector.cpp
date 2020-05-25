#include "ClientSocketConnector.h"

ClientSocketConnector::ClientSocketConnector(std::string hostname, uint16_t port) : m_port(port), m_hostname(hostname), m_open(false)
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

bool ClientSocketConnector::isOpen() {
    return m_open;
}

CallbackHandler &ClientSocketConnector::closedCallbackHandler()
{
    return m_closedCallbackHandler;
}

void ClientSocketConnector::stopClient()
{
    m_webSocket.close();
    m_webSocket.stop();
}

bool ClientSocketConnector::startClient()
{
    m_webSocket.setUrl("ws://" + m_hostname + ":" + std::to_string(m_port));
    m_webSocket.setOnMessageCallback(std::bind(&ClientSocketConnector::onMessageCallback, this, std::placeholders::_1));
    auto result = m_webSocket.connect(1);
    m_webSocket.start();
    return result.success;
}

void ClientSocketConnector::onMessageCallback(const ix::WebSocketMessagePtr &message) {
    switch(message->type) {
        case ix::WebSocketMessageType::Message:
            m_messageQueue.push(std::vector<uint8_t>(message->str.begin(), message->str.end()));
        break;
        case ix::WebSocketMessageType::Open:
            m_open = true;
            std::cout << "New connection from client" << std::endl;
        break;
        case ix::WebSocketMessageType::Close:
            m_open = false;
            std::cout << "Connection was closed" << std::endl;
        break;
        case ix::WebSocketMessageType::Error:
            m_open = false;
            std::cout << "Connection error" << message->errorInfo.reason << std::endl;
        default:
        break;
    }
}
