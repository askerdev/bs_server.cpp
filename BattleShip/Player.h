#pragma once
#include "Board.h"
#include "Socket.h"
#include <memory>

namespace app
{
	class Player
	{
	public:
		Player(std::shared_ptr<net::Socket>);

		bool IsReady();

		core::Board board;
		std::shared_ptr<net::Socket> socket;
	};
}