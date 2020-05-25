
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
        websocket->sendBinary(std::string(message.begin(), message.end()));
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

bool ServerSocketConnector::isOpen()
{
    return true;
}

void ServerSocketConnector::stopServer() {
    for(auto webSocket: m_connections) {
        webSocket->close();
    }
    m_websocketServer->stop();
}

void ServerSocketConnector::startServer() {
    m_websocketServer = new ix::WebSocketServer(m_port, "0.0.0.0");
    m_websocketServer->setOnConnectionCallback(std::bind(&ServerSocketConnector::onConnectionCallback, this, std::placeholders::_1, std::placeholders::_2));
    auto listenStatus = m_websocketServer->listen();
    if(listenStatus.first) std::cout << "listening on port 8080" << std::endl;
    else std::cout << listenStatus.second << std::endl;
    m_websocketServer->start();
}

void ServerSocketConnector::onMessageCallback(std::shared_ptr<ix::WebSocket> webSocket, const ix::WebSocketMessagePtr& message) {
    if(message->type == ix::WebSocketMessageType::Message) {
        m_messageQueue.push(std::vector<uint8_t>(message->str.begin(), message->str.end()));
    } else if(message->type == ix::WebSocketMessageType::Open) {
        m_connections.push_back(webSocket);
#ifdef QT_IS_AVAILABLE
        qInfo() << "New connection was openend";
#else
        std::cout << "New connection was openend" << std::endl;
#endif
    } else if(message->type == ix::WebSocketMessageType::Close) {
        auto result = std::find(m_connections.begin(), m_connections.end(), webSocket);
        if(result != m_connections.end()) m_connections.erase(result);
#ifdef QT_IS_AVAILABLE
        qInfo() << "Connection was closed";
#else
        std::cout << "Connection was closed" << std::endl;
#endif
    } else if(message->type == ix::WebSocketMessageType::Error) {
        std::find(m_connections.begin(), m_connections.end(), webSocket);
#ifdef QT_IS_AVAILABLE
        qWarning() << "Connection error!";
#else
        std::cout << "Connection error!" << std::endl;
#endif
    }
}

void ServerSocketConnector::onConnectionCallback(std::shared_ptr<ix::WebSocket> webSocket, std::shared_ptr<ix::ConnectionState> connectionState) {
    (void)connectionState;
    m_connections.push_back(webSocket);
    webSocket->setOnMessageCallback([=] (const ix::WebSocketMessagePtr& message) {
        onMessageCallback(webSocket, message);
    });
}
