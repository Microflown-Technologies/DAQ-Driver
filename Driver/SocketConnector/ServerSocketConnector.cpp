#include "ServerSocketConnector.h"

ServerSocketConnector::ServerSocketConnector(uint16_t port) : m_port(port)
{
    ix::initNetSystem();
    startServer();
}

ServerSocketConnector::~ServerSocketConnector() {
    stopServer();
    ix::uninitNetSystem();
}

void ServerSocketConnector::sendMessage(const std::vector<uint8_t> &message) {
    for(auto websocket: m_connections) {
        websocket->send(std::string(*message.data(), message.size()), true);
    }
}

size_t ServerSocketConnector::messagesAvailable() {
    return m_messageQueue.size();
}

std::vector<uint8_t> ServerSocketConnector::nextMessage() {
    auto message = m_messageQueue.front();
    m_messageQueue.pop();
    return message;
}

void ServerSocketConnector::stopServer() {
    for(auto webSocket: m_connections) {
        webSocket->close();
    }
    m_websocketServer->stop();
}

void ServerSocketConnector::startServer() {
    m_websocketServer = new ix::WebSocketServer(m_port);
    m_websocketServer->setOnConnectionCallback(std::bind(&ServerSocketConnector::onConnectionCallback, this, std::placeholders::_1, std::placeholders::_2));
    auto listenStatus = m_websocketServer->listen();
    std::cout << listenStatus.second << std::endl;
    m_websocketServer->start();
}

void ServerSocketConnector::onMessageCallback(const ix::WebSocketMessagePtr& message) {
    if(message->type == ix::WebSocketMessageType::Message) {
        m_messageQueue.push(std::vector<uint8_t>(*message->str.data(), message->str.size()));
    }
}

void ServerSocketConnector::onConnectionCallback(std::shared_ptr<ix::WebSocket> webSocket, std::shared_ptr<ix::ConnectionState> connectionState) {
    (void)connectionState;
    m_connections.push_back(webSocket);
    webSocket->setOnMessageCallback(std::bind(&ServerSocketConnector::onMessageCallback, this, std::placeholders::_1));
}
