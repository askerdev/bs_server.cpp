#include "Application.h"
#include "Board.h"
#include "Socket.h"
#include <exception>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>


Application::Application()
	: Server(net::Socket())
{}

void Application::Main()
{
	Start();
}

void Application::OnConnect(std::shared_ptr<net::Socket> socket)
{
	std::cout << "new connection, id: " << socket->GetID() << std::endl;
}

void Application::OnMessage(std::shared_ptr<net::Socket> socket, net::Packet packet)
{
	try
	{
		if (packet.type == net::MsgType::JOIN)
		{
			int gameid = std::stoi(packet.body);

			if (games.find(gameid) == games.end())
			{
				games[gameid] = app::Game();
			}

			auto& game = games[gameid];

			std::shared_ptr<app::Player> player = std::make_shared<app::Player>(socket);

			game.Join(player);

			(*socket) << net::MarshalState(player->board.GetState(true));

			(*socket) << net::Marshal({
				net::MsgType::INFO,
				"You joined a game with id " + std::to_string(gameid),
				});
			return;
		}

		auto& game = games[FindGameByPlayerID(socket->GetID())];

		if (game.IsEnd())
		{
			(*socket) << net::Marshal({
						net::MsgType::INFO,
						"end game!",
				});
		} else if (packet.type == net::MsgType::PLACESHIP)
		{
			auto player = game.GetPlayer(socket->GetID());
			auto ship = ParseShip(packet.body);
			player->board.AddShip(ship);
			game.CheckReadyAndUpdateState();

			(*socket) << net::MarshalState(player->board.GetState(true));

			if (game.IsFull() && game.CanShoot())
			{
				(*(game.Opponent(player)->socket)) << net::Marshal({
						net::MsgType::INFO,
						"you can play!",
					});
				(*socket) << net::Marshal({
						net::MsgType::INFO,
						"you can play!",
					});
			}
		} else if (packet.type = net::MsgType::MOVE)
		{
			auto player = game.GetPlayer(socket->GetID());
			if (!player->IsReady())
			{
				throw std::exception("place all ships before shooting");
			}
			if (!game.CanShoot(socket->GetID()))
			{
				throw std::exception("wait for your opponent");
			}
			auto shoot = net::ParseShoot(packet);
			const bool& hit = game.Shoot(shoot.first, shoot.second);
			const std::string msg = hit ? "hit!" : "miss!";
			auto opponent = game.Opponent(player);

			(*(opponent->socket)) << net::MarshalState({
				opponent->board.GetState(true),
				player->board.GetState(),
				});
			(*socket) << net::MarshalState({
				player->board.GetState(true),
				opponent->board.GetState(),
				});

			if (game.GetWinner() != nullptr)
			{
				auto winner = game.GetWinner();

				if (winner->socket->GetID() == socket->GetID())
				{
					(*(opponent->socket)) << net::Marshal({
							net::MsgType::INFO,
							"GAME OVER!",
						});
					(*socket) << net::Marshal({
							net::MsgType::INFO,
							"WINNER WINNER CHICKEN DINNER!",
						});
				} else
				{
					(*socket) << net::Marshal({
							net::MsgType::INFO,
							"GAME OVER!",
						});
					(*(opponent->socket)) << net::Marshal({
						   net::MsgType::INFO,
						   "WINNER WINNER CHICKEN DINNER!",
						});
				}
			} else
			{
				(*socket) << net::Marshal({
					net::MsgType::INFO,
					msg,
					});
				if (!hit)
				{
					(*(opponent)->socket) << net::Marshal(
						{
							net::MsgType::INFO,
							"YOU CAN SHOOT!",
						}
						);
				}
			}
		} else if (packet.type = net::MsgType::EXIT)
		{
			games.erase(FindGameByPlayerID(socket->GetID()));
		} else
		{
			throw std::exception("unknown message type");
		}
	} catch (const std::exception& e)
	{
		(*socket) << net::Marshal({
				net::MsgType::INFO,
				e.what(),
			});
	}
}

void Application::OnDisconnect(std::shared_ptr<net::Socket> socket)
{
	for (auto& pair : games)
	{
		if (pair.second.HavePlayer(socket->GetID()))
		{
			games.erase(pair.first);
			break;
		}
	}

	std::cout << "disconnected, id: " << socket->GetID() << std::endl;
}

int Application::FindGameByPlayerID(const int& id)
{
	for (auto& pair : games)
	{
		if (pair.second.HavePlayer(id))
		{
			return pair.first;
		}
	}
	throw std::exception("join a game before");
}