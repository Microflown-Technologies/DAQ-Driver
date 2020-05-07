#include "ClientSocketConnector.h"

ClientSocketConnector::ClientSocketConnector(std::string hostname)
{

}

void ClientSocketConnector::sendMessage(const std::vector<uint8_t> &message) {
    m_webSocket.send(std::string(*message.data(), message.size()), true);
}

size_t ClientSocketConnector::messagesAvailable() {
    return m_messageQueue.size();
}

std::vector<uint8_t> ClientSocketConnector::nextMessage() {
    auto message = m_messageQueue.front();
    m_messageQueue.pop();
    return message;
}

void ClientSocketConnector::startClient()
{
    m_webSocket.setUrl("ws://" + m_hostname + "/");
    m_webSocket.setOnMessageCallback(std::bind(&ClientSocketConnector::onMessageCallback, this, std::placeholders::_1));
}

void ClientSocketConnector::onMessageCallback(const ix::WebSocketMessagePtr &message) {
    if(message->type == ix::WebSocketMessageType::Message) {
        m_messageQueue.push(std::vector<uint8_t>(*message->str.data(), message->str.size()));
    }
}
