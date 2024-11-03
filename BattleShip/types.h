#pragma once

namespace core
{
	enum CellState
	{
		BLOWN = 0,
		HIT = 1,
		FREE = 2,
		MISS = 3,
		SHIP = 4,
	};

	enum ShipType
	{
		BOAT = 1,
		DESTROYER = 2,
		CRUISER = 3,
		BATTLESHIP = 4,
	};

	int GetShipCountByType(core::ShipType type);

	enum Orientation
	{
		UNKNOWN = 0,
		HORIZONTAL = 1,
		VERTICAL = 2,
	};

	bool IsValidCell(int x, int y);
}
