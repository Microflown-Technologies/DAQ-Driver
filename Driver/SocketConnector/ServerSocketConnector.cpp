
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

bool ServerSocketConnector::isOpen() {
    return !m_connections.empty();
}

CallbackHandler &ServerSocketConnector::closedCallbackHandler() {
    return m_closedCallbackHandler;
}

CallbackHandler &ServerSocketConnector::openedCallbackHandler() {
    return m_openendCallbackHandler;
}

uint16_t ServerSocketConnector::port() {
    return m_port;
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
    if(listenStatus.first) {
#ifdef QT_IS_AVAILABLE
        qInfo() << "DAQDriver listening on port"  << m_port;
#else
        std::cout << "DAQDriver listening on port"  << std::to_string(m_port) << std::endl;
#endif
    }
    else std::cout << listenStatus.second << std::endl;
    m_websocketServer->start();
}

void ServerSocketConnector::onMessageCallback(std::shared_ptr<ix::WebSocket> webSocket, const ix::WebSocketMessagePtr& message) {
    if(message->type == ix::WebSocketMessageType::Message) {
        m_messageQueue.push(std::vector<uint8_t>(message->str.begin(), message->str.end()));
    } else if(message->type == ix::WebSocketMessageType::Open) {
        for(auto webSocket: m_connections) {
            webSocket->close();
        }
        m_connections.clear();
        webSocket->setMaxWaitBetweenReconnectionRetries(100);
        webSocket->enablePong();
        m_connections.push_back(webSocket);
        m_openendCallbackHandler.invokeCallbacks();
#ifdef QT_IS_AVAILABLE
        qInfo() << "New connection was openend";
#else
        std::cout << "New connection was openend" << std::endl;
#endif
    } else if(message->type == ix::WebSocketMessageType::Close) {
        auto result = std::find(m_connections.begin(), m_connections.end(), webSocket);
        if(result != m_connections.end()) m_connections.erase(result);
        else std::cout << "trying to delete connection that does not exist!" << std::endl;
#ifdef QT_IS_AVAILABLE
        qInfo() << "Connection was closed";
#else
        std::cout << "Connection was closed" << std::endl;
#endif
        if(isOpen()) {
            closedCallbackHandler().invokeCallbacks();
        }
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
    qDebug() << "Terminated" << connectionState->isTerminated();
    webSocket->setOnMessageCallback([=] (const ix::WebSocketMessagePtr& message) {
        onMessageCallback(webSocket, message);
    });
}
