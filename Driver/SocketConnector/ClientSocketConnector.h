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
#include <ixwebsocket/IXWebSocket.h>

class ClientSocketConnector : public AbstractSocketConnector
{
public:
    ClientSocketConnector(std::string hostname);
    ~ClientSocketConnector();


    virtual void sendMessage(const std::vector<uint8_t> &message) override;

    virtual size_t messagesAvailable() override;

    virtual std::vector<uint8_t> nextMessage() override;

protected:
    void stopClient();
    void startClient();
    void onMessageCallback(const ix::WebSocketMessagePtr &message);

private:
    std::string m_hostname;
    ix::WebSocket m_webSocket;
    std::queue<std::vector<uint8_t>> m_messageQueue;
};

#endif // CLIENTSOCKETCONNECTOR_H
