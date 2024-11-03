#include "Packet.h"
#include "Socket.h"
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <winsock2.h>
#include <ws2def.h>
#include <ws2tcpip.h>

net::Socket::Socket(SOCKET fd)
	: fd(fd), id(0)
{}

net::Socket::~Socket()
{
	closesocket(fd);
}

void net::Socket::Listen(const int& port)
{
	addrinfo hints{}, * ai;

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if ((getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &ai)) != 0)
	{
		throw std::invalid_argument("invalid port");
	}
	fd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
	if (fd < 0)
	{
		throw std::invalid_argument("get socket");
	}
	/*int yes = 1;
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, 8);*/
	if (bind(fd, ai->ai_addr, ai->ai_addrlen) < 0)
	{
		closesocket(fd);
		throw std::invalid_argument("binding");
	}

	freeaddrinfo(ai);

	if (listen(fd, 10) == -1)
	{
		throw std::invalid_argument("listening");
	}
}

void net::Socket::Connect(const std::string& host, const int& port)
{
	addrinfo hints{}, * ai;

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if ((getaddrinfo(host.c_str(), std::to_string(port).c_str(), &hints, &ai)) != 0)
	{
		throw std::invalid_argument("invalid port");
	}
	fd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
	if (fd < 0)
	{
		throw std::invalid_argument("get socket");
	}
	if (connect(fd, ai->ai_addr, ai->ai_addrlen) == -1)
	{
		closesocket(fd);
		throw std::invalid_argument("while connecting");
	}
	freeaddrinfo(ai);
};

void net::Socket::Close()
{
	closed = true;
}

std::shared_ptr<net::Socket> net::Socket::Accept()
{
	sockaddr_storage remoteaddr{};
	int addrlen = sizeof(remoteaddr);
	SOCKET newfd = accept(fd, (sockaddr*)&remoteaddr, &addrlen);
	return std::make_shared<net::Socket>(newfd);
}

int net::Socket::GetID()
{
	return id;
}

void net::Socket::SetID(const int& id)
{
	this->id = id;
}

void net::Socket::Write(const std::string& in)
{
	if ((send(fd, in.c_str(), in.size(), 0)) == -1 || closed)
	{
		throw std::invalid_argument("error or socket closed");
	}
}

std::string net::Socket::Read()
{
	char buf[TOTAL_PACKET_SIZE + 1];
	int nbytes = recv(fd, buf, sizeof(buf) - 1, 0);
	if (nbytes <= 0 || closed)
	{
		throw std::invalid_argument("error or socket closed");
	}
	buf[nbytes] = '\0';
	return std::string(buf);
}

SOCKET net::Socket::GetFD()
{
	return fd;
}

net::Socket& net::operator<<(net::Socket& sock, const std::string& in)
{
	if ((send(sock.fd, in.c_str(), in.size(), 0)) == -1 || sock.closed)
	{
		throw std::invalid_argument("error or socket closed");
	}
	return sock;
};

net::Socket& net::operator>>(net::Socket& sock, std::string& out)
{
	char buf[TOTAL_PACKET_SIZE + 1];
	int nbytes = recv(sock.fd, buf, sizeof(buf) - 1, 0);
	if (nbytes <= 0 || sock.closed)
	{
		throw std::invalid_argument("error or socket closed");
	}
	buf[nbytes] = '\0';
	return sock;
};

