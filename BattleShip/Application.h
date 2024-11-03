#pragma once
#include "Game.h"
#include "Packet.h"
#include "Server.h"
#include <unordered_map>

class Application : public Server
{
public:
	Application();
	void Main();

	void OnConnect(std::shared_ptr<net::Socket>) override;
	void OnMessage(std::shared_ptr<net::Socket>, net::Packet) override;
	void OnDisconnect(std::shared_ptr<net::Socket>) override;

protected:
	int FindGameByPlayerID(const int&);
	std::unordered_map<int, app::Game> games;
};

