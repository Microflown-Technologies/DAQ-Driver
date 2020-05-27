#ifndef CLIENTSOCKETCONNECTOR_H
#define CLIENTSOCKETCONNECTOR_H

//STD framework
#include <queue>
#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include <stdint.h>

//Internal headers
#include "AbstractSocketConnector.h"

//IXWebSocket
#include "ixwebsocket/IXNetSystem.h"
#include <ixwebsocket/IXWebSocket.h>

class ClientSocketConnector : public AbstractSocketConnector
{
public:
    ClientSocketConnector(std::string hostname = AbstractSocketConnector::defaultIpAddress, uint16_t port = AbstractSocketConnector::defaultPort);
    ~ClientSocketConnector();


    virtual void sendMessage(const std::vector<uint8_t> &message) override;

    virtual size_t messagesAvailable() override;

    virtual std::vector<uint8_t> nextMessage() override;

    virtual bool isOpen() override;

    virtual CallbackHandler & closedCallbackHandler() override;

    virtual uint16_t port() override;

protected:
    void stopClient();
    bool startClient();
    void onMessageCallback(const ix::WebSocketMessagePtr &message);

private:
    uint16_t m_port;
    std::string m_hostname;
    std::atomic_bool m_open;
    ix::WebSocket m_webSocket;
    std::queue<std::vector<uint8_t>> m_messageQueue;
};

#endif // CLIENTSOCKETCONNECTOR_H
