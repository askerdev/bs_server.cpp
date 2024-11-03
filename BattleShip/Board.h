#pragma once

#include "Ship.h"
#include "types.h"
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace core
{
	class Board
	{
	public:
		Board();

		bool IsValid();

		void AddShip(core::Ship& ship);

		std::pair<bool, std::string> IsFreeToPlace(core::Ship&, int, int);

		bool IsValidShot(int tryX, int tryY);

		bool Shoot(int tryX, int tryY);

		bool IsAllShipsSunk();

		std::string GetState(const bool& self = false);

		void Render();

	private:
		std::vector<std::pair<int, int>> shootHistory;
		std::vector<std::vector<core::CellState>> board;
		std::unordered_map<core::ShipType, std::vector<core::Ship>> ships;
		std::vector<std::vector<int>> shipMap;
	};
}

template <typename T>
std::string join(const std::vector<T>& in, const std::string& sep = ",");

std::vector<std::string> split(const std::string& in, const char sep = ',');