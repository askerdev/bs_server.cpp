#include "types.h"

int core::GetShipCountByType(core::ShipType type)
{
	switch (type)
	{
	case core::ShipType::BOAT:
		return 4;
	case core::ShipType::DESTROYER:
		return 3;
	case core::ShipType::CRUISER:
		return 2;
	case core::ShipType::BATTLESHIP:
		return 1;
	}
}

bool core::IsValidCell(int x, int y)
{
	return 0 <= x && x <= 9 && 0 <= y && y <= 9;
}
