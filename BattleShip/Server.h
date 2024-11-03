#pragma once
#include "Packet.h"
#include "Socket.h"
#include <memory>
#include <queue>
#include <string>
#include <utility>

class Server
{
public:
	Server(net::Socket);

	void Start();

	virtual void OnConnect(std::shared_ptr<net::Socket>) = 0;
	virtual void OnMessage(std::shared_ptr<net::Socket>, net::Packet) = 0;
	virtual void OnDisconnect(std::shared_ptr <net::Socket>) = 0;
protected:
	std::queue<std::pair<std::shared_ptr<net::Socket>, net::Packet>> packetQueue;
private:
	net::Socket sock;
	int currentId = 1;
};