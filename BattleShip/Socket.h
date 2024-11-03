#pragma once
#include "CircularBuffer.h"
#include "Packet.h"
#include <memory>
#include <string>
#include <winsock2.h>

namespace net
{
	class Socket
	{
	public:
		Socket() = default;
		Socket(SOCKET);
		~Socket();

		void Listen(const int&);
		void Connect(const std::string&, const int&);
		void Close();

		std::shared_ptr<net::Socket> Accept();
		SOCKET GetFD();
		int GetID();
		void SetID(const int&);
		void Write(const std::string&);
		std::string Read();

		friend net::Socket& operator<<(net::Socket&, const std::string&);
		friend net::Socket& operator>>(net::Socket&, std::string&);
	private:
		bool closed = false;
		SOCKET fd;
		int id;
	};
}