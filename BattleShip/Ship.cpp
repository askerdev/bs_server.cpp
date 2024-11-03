#include "Ship.h"
#include "types.h"
#include <stdexcept>
#include <string>


core::Ship::Ship(int startX, int startY, core::Orientation orientation, core::ShipType type)
{
	this->strikeCount = 0;
	this->alive = true;
	this->startX = startX;
	this->startY = startY;
	this->type = type;
	this->orientation = orientation;

	switch (orientation)
	{
	case core::Orientation::UNKNOWN:
		this->endX = startX;
		this->endY = startY;
		break;
	case core::Orientation::VERTICAL:
		this->endX = startX;
		this->endY = startY + type - 1;
		break;
	case core::Orientation::HORIZONTAL:
		this->endX = startX + type - 1;
		this->endY = startY;
		break;
	}
}

core::Ship::Ship(int startX, int startY)
{
	this->strikeCount = 0;
	this->alive = true;
	this->type = core::ShipType::BOAT;
	this->startX = startX;
	this->startY = startY;
	this->endX = startX;
	this->endY = startY;
	this->orientation = core::Orientation::UNKNOWN;
}

bool core::Ship::Shoot(int tryX, int tryY)
{
	const bool hit = Overlap(tryX, tryY);

	if (alive && hit)
		strikeCount++;

	if (strikeCount == int(type) && alive)
		alive = false;

	return hit;
}

bool core::Ship::IsAlive()
{
	return alive;
}

bool core::Ship::Overlap(int x, int y)
{
	return startX <= x && x <= endX && startY <= y && y <= endY;
}

core::Orientation core::Ship::GetOrientation() { return orientation; }
core::ShipType core::Ship::GetType() { return type; }

int core::Ship::GetStartX() { return startX; }
int core::Ship::GetStartY() { return startY; }
int core::Ship::GetEndX() { return endX; }
int core::Ship::GetEndY() { return endY; }


core::Ship ParseShip(const std::string& data)
{
	core::Orientation orientation;
	core::ShipType shipType;
	int x = data[2] - '0';
	int y = data[3] - '0';

	switch (data[0])
	{
	case '0':
		orientation = core::Orientation::UNKNOWN;
		break;
	case '1':
		orientation = core::Orientation::HORIZONTAL;
		break;
	case '2':
		orientation = core::Orientation::VERTICAL;
		break;
	default:
		throw std::invalid_argument("invalid data");
	}

	switch (data[1])
	{
	case '1':
		shipType = core::ShipType::BOAT;
		break;
	case '2':
		shipType = core::ShipType::DESTROYER;
		break;
	case '3':
		shipType = core::ShipType::CRUISER;
		break;
	case '4':
		shipType = core::ShipType::BATTLESHIP;
		break;
	default:
		throw std::invalid_argument("invalid data");
	}

	if (!core::IsValidCell(x, y))
	{
		throw std::invalid_argument("invalid place");
	}

	if (orientation == core::Orientation::UNKNOWN && shipType != core::ShipType::BOAT)
	{
		throw std::invalid_argument("only boats can have unknown orientation");
	}

	return {
		x,
		y,
		orientation,
		shipType,
	};
}