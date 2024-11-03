#include "Packet.h"
#include "types.h"
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

std::string net::Marshal(net::Packet packet)
{
	return std::to_string(packet.type) + packet.body + ";";
}

std::string net::MarshalState(std::pair<std::string, std::string> states)
{
	return net::Marshal({
			net::MsgType::STATE,
			"[" + states.first + "|" + states.second + "]",
		});
}
std::string net::MarshalState(std::string state)
{
	return net::Marshal({
			net::MsgType::STATE,
			"[" + state + "|]",
		});
}

net::Packet net::Unmarshal(std::string in)
{
	net::MsgType type;
	switch (in[0])
	{
	case '1':
		type = net::MsgType::JOIN;
		break;
	case '2':
		type = net::MsgType::MOVE;
		break;
	case '3':
		type = net::MsgType::EXIT;
		break;
	case '4':
		type = net::MsgType::PLACESHIP;
		break;
	case '5':
		type = net::MsgType::INFO;
		break;
	default:
		throw std::invalid_argument("invlaid data");
	}
	return{ type, in.substr(1, in.size() - 2) };
}

std::pair<int, int> net::ParseShoot(net::Packet packet)
{
	if (packet.type != net::MsgType::MOVE)
	{
		throw std::invalid_argument("invalid packet type");
	}
	int x = packet.body[0] - '0';
	int y = packet.body[1] - '0';

	if (!core::IsValidCell(x, y))
	{
		throw std::exception("invalid coordinates");
	}

	return { x,y };
}