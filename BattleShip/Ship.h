#pragma once
#include "types.h"
#include <string>

namespace core
{
	class Ship
	{
	public:
		Ship(int startX, int startY, core::Orientation orientation, core::ShipType type);

		Ship(int startX, int startY);

		bool Shoot(int tryX, int tryY);

		bool IsAlive();
		bool Overlap(int, int);

		core::Orientation GetOrientation();
		core::ShipType GetType();

		int GetStartX();
		int GetStartY();
		int GetEndX();
		int GetEndY();

	private:
		int startX;
		int startY;
		int endX;
		int endY;
		int strikeCount;
		core::ShipType type;
		bool alive;
		core::Orientation orientation;
	};
}

core::Ship ParseShip(const std::string&);