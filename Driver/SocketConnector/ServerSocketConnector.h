#ifndef SERVERSOCKETCONNECTOR_H
#define SERVERSOCKETCONNECTOR_H

//STD framework
#include <queue>
#include <vector>
#include <memory>
#include <string>
#include <stdint.h>

//Internal headers
#include "AbstractSocketConnector.h"

//IXWebsocket
#include "ixwebsocket/IXNetSystem.h"
#include "ixwebsocket/IXWebSocketServer.h"
#include "ixwebsocket/IXWebSocketMessage.h"

class ServerSocketConnector : public AbstractSocketConnector
{
public:
    ServerSocketConnector(uint16_t port = 80);
    ~ServerSocketConnector();

    virtual void sendMessage(const std::vector<uint8_t> &message) override;

    virtual size_t messagesAvailable() override;

    virtual std::vector<uint8_t> nextMessage() override;

protected:
    void stopServer();
    void startServer();
    void onMessageCallback(const ix::WebSocketMessagePtr &message);
    void onConnectionCallback(std::shared_ptr<ix::WebSocket> webSocket, std::shared_ptr<ix::ConnectionState> connectionState);

private:
    uint16_t m_port;
    ix::WebSocketServer *m_websocketServer;
    std::queue<std::vector<uint8_t>> m_messageQueue;
    std::vector<std::shared_ptr<ix::WebSocket>> m_connections;
};

#endif // SERVERSOCKETCONNECTOR_H
