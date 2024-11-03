#include "CircularBuffer.h"
#include "Packet.h"
#include "Server.h"
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <winsock2.h>
#include <ws2tcpip.h>

Server::Server(net::Socket sock) :
	sock(sock)
{}

void Server::Start()
{
	sock.Listen(8080);

	fd_set master{};
	fd_set read_fds{};

	FD_ZERO(&read_fds);
	FD_ZERO(&master);

	FD_SET(sock.GetFD(), &master);

	SOCKET fdmax = sock.GetFD();

	std::unordered_map<SOCKET,
		std::pair<std::shared_ptr<net::CircularBuffer<char>>, std::shared_ptr<net::Socket>>
	> clients;

	while (true)
	{
		while (packetQueue.size())
		{
			try
			{
				OnMessage(packetQueue.front().first, packetQueue.front().second);
			} catch (const std::exception&)
			{
				packetQueue.front().first->Close();
			}
			packetQueue.pop();
		}

		read_fds = master;

		if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1)
		{
			throw std::invalid_argument("while select");
		}
		for (SOCKET fd = 0; fd <= fdmax; fd++)
		{
			if (!FD_ISSET(fd, &read_fds))
			{
				continue;
			}

			if (fd == sock.GetFD())
			{
				auto client = sock.Accept();
				client->SetID(currentId++);
				OnConnect(client);
				clients[client->GetFD()] = {
					std::make_shared<net::CircularBuffer<char>>(TOTAL_PACKET_SIZE),
					client,
				};
				FD_SET(client->GetFD(), &master);
				if (client->GetFD() > fdmax)
				{
					fdmax = client->GetFD();
				}
			} else
			{
				try
				{
					std::string data = clients[fd].second->Read();
					auto& buf = clients[fd].first;
					for (auto& symbol : data)
					{
						try
						{
							buf->Write(symbol);
						} catch (const std::exception&)
						{
							buf->Erase();
							continue;
						}
						if (symbol == ';')
						{
							std::string chunk;
							do
							{
								chunk += buf->Read();
							} while (chunk.back() != ';');
							try
							{
								packetQueue.push({
									clients[fd].second,
									net::Unmarshal(chunk)
									});
							} catch (const std::exception&)
							{
								continue;
							}
						}
					}
				} catch (const std::exception&)
				{
					OnDisconnect(clients[fd].second);
					FD_CLR(fd, &master);
					clients.erase(fd);
				}
			}
		}
	}
}
