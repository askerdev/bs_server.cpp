#include "Player.h"
#include "Socket.h"
#include <memory>

app::Player::Player(std::shared_ptr<net::Socket> socket)
	: socket(socket)
{}

bool app::Player::IsReady()
{
	return board.IsValid();
}