#pragma once
#include "Player.h"
#include <memory>

namespace app
{
	enum GameState
	{
		INIT = 1,
		MOVE_LEFT = 2,
		MOVE_RIGHT = 3,
		END = 4,
	};

	class Game
	{
	public:
		Game();
		~Game();
		void Join(std::shared_ptr<app::Player>);
		void CheckReadyAndUpdateState();
		bool HavePlayer(const int&);
		bool Shoot(int, int);
		bool IsFull();
		bool IsEnd();
		bool CanShoot();
		bool CanShoot(const int&);
		void ToggleMove();
		std::shared_ptr<app::Player> GetWinner();
		std::shared_ptr<app::Player> GetPlayer(const int&);
		std::shared_ptr<app::Player> Opponent(std::shared_ptr<app::Player>);
	private:
		app::GameState state;
		std::shared_ptr<app::Player> leftPlayer;
		std::shared_ptr <app::Player> rightPlayer;
	};
}
