#pragma once

#include "Board.h"
#include "Ship.h"
#include "types.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

core::Board::Board() :
	board(10, std::vector<core::CellState>(10, core::CellState::FREE)),
	shipMap(10, std::vector<int>(10, -1))
{
	std::vector<Ship> arr1;
	std::vector<Ship> arr2;
	std::vector<Ship> arr3;
	std::vector<Ship> arr4;
	arr1.reserve(core::GetShipCountByType(core::ShipType::BOAT));
	ships[core::ShipType::BOAT] = arr1;
	arr2.reserve(core::GetShipCountByType(core::ShipType::DESTROYER));
	ships[core::ShipType::DESTROYER] = arr2;
	arr3.reserve(core::GetShipCountByType(core::ShipType::CRUISER));
	ships[core::ShipType::CRUISER] = arr3;
	arr4.reserve(core::GetShipCountByType(core::ShipType::BATTLESHIP));
	ships[core::ShipType::BATTLESHIP] = arr4;
}

bool core::Board::IsValid()
{
	for (auto& entry : ships)
	{
		if (entry.second.size() != core::GetShipCountByType(entry.first))
		{
			return false;
		}
	}
	return true;
}

void core::Board::AddShip(core::Ship& ship)
{
	if (ships[ship.GetType()].size() == core::GetShipCountByType(ship.GetType()))
		throw std::exception("all available ship with this type are placed");

	int shipId = ships.size();
	std::vector<std::pair<int, int>> coords;

	for (int y = ship.GetStartY(); y <= ship.GetEndY(); y++)
	{
		for (int x = ship.GetStartX(); x <= ship.GetEndX(); x++)
		{
			auto res = IsFreeToPlace(ship, x, y);
			if (res.first)
			{
				coords.push_back({ x, y });
			} else
			{
				throw std::exception(res.second.c_str());
			}
		}
	}

	ships[ship.GetType()].emplace_back(ship);
	for (auto& coord : coords)
	{
		shipMap[coord.second][coord.first] = shipId;
		board[coord.second][coord.first] = core::CellState::SHIP;
	}

	return;
}

std::pair<bool, std::string>core::Board::IsFreeToPlace(core::Ship& ship, int x, int y)
{
	for (int diffY = -1; diffY <= 1; diffY++)
	{
		for (int diffX = -1; diffX <= 1; diffX++)
		{
			if (!core::IsValidCell(x + diffX, y + diffY) && ship.Overlap(x + diffX, y + diffY))
			{
				return { false, "invalid position for your ship" };
			} else if (!core::IsValidCell(x + diffX, y + diffY))
			{
				continue;
			}
			if (shipMap[y + diffY][x + diffX] != -1)
			{
				return { false, "place already taken or gap between ships are too small" };
			}
		}
	}
	return { true,"" };
}

bool core::Board::IsValidShot(int tryX, int tryY)
{
	if (!core::IsValidCell(tryX, tryY))
	{
		return false;
	}
	if (board[tryY][tryX] == core::CellState::BLOWN ||
		board[tryY][tryX] == core::CellState::HIT ||
		board[tryY][tryX] == core::CellState::MISS)
	{
		return false;
	}
	return true;
}

bool core::Board::Shoot(int tryX, int tryY)
{
	if (!IsValidShot(tryX, tryY))
	{
		throw std::exception("invalid shoot");
	}

	bool isShot = false;
	for (auto& entry : ships)
	{
		for (auto& ship : entry.second)
		{
			if (!ship.Shoot(tryX, tryY))
			{
				continue;
			}
			if (!ship.IsAlive())
			{
				for (int y = ship.GetStartY(); y <= ship.GetEndY(); y++)
				{
					for (int x = ship.GetStartX(); x <= ship.GetEndX(); x++)
					{
						board[y][x] = core::CellState::BLOWN;
					}
				}
			} else
			{
				board[tryY][tryX] = core::CellState::HIT;
			}
			isShot = true;
			break;
		}
		if (isShot)
			break;
	}
	if (!isShot)
	{
		board[tryY][tryX] = core::CellState::MISS;
	} else
	{
		shootHistory.push_back({ tryX, tryY });
	}

	return isShot;
}

bool core::Board::IsAllShipsSunk()
{
	for (auto& entry : ships)
	{
		for (auto& ship : entry.second)
		{
			if (ship.IsAlive())
			{
				return false;
			}
		}
	}
	return true;
}

void core::Board::Render()
{
	std::cout << std::setw(3) << "";
	for (int i = 65; i < 75; i++)
	{
		std::cout << char(i) << " ";
	}

	std::cout << std::endl;

	for (int i = 0; i < board.size(); i++)
	{
		auto& row = board[i];
		std::cout << std::setw(2) << i + 1 << " ";
		for (auto& cell : row)
		{
			switch (cell)
			{
			case core::CellState::BLOWN:
				std::cout << "#";
				break;
			case core::CellState::HIT:
				std::cout << "X";
				break;
			case core::CellState::FREE:
				std::cout << "_";
				break;
			case core::CellState::MISS:
				std::cout << "o";
				break;
			}
			std::cout << " ";
		}
		std::cout << std::endl;
	}
}

std::string core::Board::GetState(const bool& self)
{
	std::vector<std::string> stateVector;
	for (int y = 0; y < 10; y++)
	{
		auto& row = board[y];
		for (int x = 0; x < 10; x++)
		{
			auto& cellState = row[x];
			if (cellState == core::CellState::FREE ||
				(cellState == core::CellState::SHIP && !self))
			{
				continue;
			}
			stateVector.push_back(std::to_string(x) + std::to_string(y) + std::to_string(cellState));
		}
	}
	return join(stateVector);
}

template <typename T>
std::string join(const std::vector<T>& in, const std::string& sep)
{
	std::ostringstream stream;
	for (int i = 0; i < in.size(); i++)
	{
		if (i) stream << sep;
		stream << in[i];
	}
	return stream.str();
}

std::vector<std::string> split(const std::string& in, const char sep)
{
	std::vector<std::string> result;
	std::istringstream ss(in);
	std::string item;

	while (std::getline(ss, item, sep))
	{
		result.push_back(item);
	}

	return result;
}