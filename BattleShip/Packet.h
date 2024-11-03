#pragma once
#include <string>
#include <utility>

#define TOTAL_PACKET_SIZE 4100
#define PACKET_HEADERS_SIZE 4
#define PACKET_BODY_SIZE 4096

namespace net
{
	enum MsgType
	{
		STATE = 0, // 0:[xystate,xystate]|[xystate,xystate];
		JOIN = 1, // 1:gameid;
		MOVE = 2, // 2:xy;
		EXIT = 3, // 3:;
		PLACESHIP = 4, // 4:Orientation,ShipType,startX,startY;
		INFO = 5,
	};

	struct Packet
	{
		MsgType type;
		std::string body;
	};

	std::string Marshal(net::Packet packet);
	std::string MarshalState(std::pair<std::string, std::string>);
	std::string MarshalState(std::string);
	net::Packet Unmarshal(std::string in);
	std::pair<int, int> ParseShoot(net::Packet packet);
}
