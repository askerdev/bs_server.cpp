#include "Game.h"
#include "Player.h"
#include <exception>

app::Game::Game() :
	state(app::GameState::INIT),
	leftPlayer(nullptr),
	rightPlayer(nullptr)
{}

app::Game::~Game()
{
	if (leftPlayer != nullptr)
	{
		leftPlayer->socket->Close();
	}
	if (rightPlayer != nullptr)
	{
		rightPlayer->socket->Close();
	}
}

bool app::Game::IsEnd()
{
	return state == app::GameState::END;
}

void app::Game::Join(std::shared_ptr<app::Player> player)
{
	if (leftPlayer == nullptr)
	{
		leftPlayer = player;
	} else if (rightPlayer == nullptr)
	{
		rightPlayer = player;
	} else
	{
		throw std::exception("game is full");
	}
}

void app::Game::CheckReadyAndUpdateState()
{
	if (leftPlayer == nullptr || rightPlayer == nullptr)
	{
		return;
	}

	if (leftPlayer->board.IsValid() && rightPlayer->board.IsValid())
	{
		state = app::GameState::MOVE_LEFT;
	}
}

void app::Game::ToggleMove()
{
	if (state == app::GameState::MOVE_LEFT)
	{
		state = app::GameState::MOVE_RIGHT;
	} else if (state == app::GameState::MOVE_RIGHT)
	{
		state = app::GameState::MOVE_LEFT;
	}
}

std::shared_ptr<app::Player> app::Game::GetWinner()
{
	if (leftPlayer == nullptr || rightPlayer == nullptr)
	{
		throw std::exception("game is not full");
	}

	if (leftPlayer->board.IsAllShipsSunk())
	{
		state = app::GameState::END;
		return rightPlayer;
	}

	if (rightPlayer->board.IsAllShipsSunk())
	{
		state = app::GameState::END;
		return leftPlayer;
	}

	return nullptr;
}

bool app::Game::HavePlayer(const int& id)
{
	if (leftPlayer != nullptr && leftPlayer->socket->GetID() == id)
	{
		return true;
	}
	if (rightPlayer != nullptr && rightPlayer->socket->GetID() == id)
	{
		return true;
	}
	return false;
}

bool app::Game::Shoot(int x, int y)
{
	if (leftPlayer == nullptr || rightPlayer == nullptr)
	{
		throw std::exception("game is not full");
	}

	bool isShot = false;
	switch (state)
	{
	case app::GameState::MOVE_LEFT:
		isShot = rightPlayer->board.Shoot(x, y);
		break;
	case app::GameState::MOVE_RIGHT:
		isShot = leftPlayer->board.Shoot(x, y);
		break;
	case app::GameState::INIT:
		throw std::exception("wait for your opponent to be ready!");
		break;
	case app::GameState::END:
		throw std::exception("end game!");
		break;
	}

	if (leftPlayer->board.IsAllShipsSunk() || rightPlayer->board.IsAllShipsSunk())
	{
		state = app::GameState::END;
	} else if (!isShot)
	{
		ToggleMove();
	}

	return isShot;
}

bool app::Game::IsFull()
{
	return leftPlayer != nullptr && rightPlayer != nullptr;
}

bool app::Game::CanShoot()
{
	if (leftPlayer == nullptr || rightPlayer == nullptr)
	{
		throw std::exception("game is not full");
	}
	return leftPlayer->IsReady() && rightPlayer->IsReady();
}

bool app::Game::CanShoot(const int& id)
{
	if (leftPlayer == nullptr || rightPlayer == nullptr)
	{
		throw std::exception("game is not full");
	}
	if (!leftPlayer->IsReady() || !rightPlayer->IsReady())
	{
		return false;
	}

	switch (state)
	{
	case app::GameState::MOVE_LEFT:
		return leftPlayer->socket->GetID() == id;
	case app::GameState::MOVE_RIGHT:
		return rightPlayer->socket->GetID() == id;
	default:
		return false;
	}
}

std::shared_ptr<app::Player> app::Game::GetPlayer(const int& id)
{
	if (leftPlayer != nullptr && leftPlayer->socket->GetID() == id)
	{
		return leftPlayer;
	}
	if (rightPlayer != nullptr && rightPlayer->socket->GetID() == id)
	{
		return rightPlayer;
	}
	throw std::exception("player not found");
}

std::shared_ptr<app::Player> app::Game::Opponent(std::shared_ptr<app::Player> player)
{
	if (leftPlayer == nullptr || rightPlayer == nullptr)
	{
		throw std::exception("game is not full");
	}

	if (leftPlayer->socket->GetID() == player->socket->GetID())
	{
		return rightPlayer;
	} else if (rightPlayer->socket->GetID() == player->socket->GetID())
	{
		return leftPlayer;
	}

	throw std::exception("player does not exist in this game");
}
