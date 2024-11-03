#include "CircularBuffer.h"
#include <exception>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

net::CircularBuffer<char>::CircularBuffer(const int& max_size)
	: max_size(max_size),
	head(0),
	tail(0),
	full(false),
	buf(std::unique_ptr<T[]>(new T[max_size]))
{}

char net::CircularBuffer<char>::Read()
{
	const char val = buf[tail];
	tail = (tail + 1) % max_size;
	return val;
}

void net::CircularBuffer<char>::Erase()
{
	head = 0;
	tail = 0;
	full = false;
	buf = std::unique_ptr<char[]>(new char[max_size]);
}

bool net::CircularBuffer<char>::IsFull()
{
	return full;
}

void net::CircularBuffer<char>::Write(char val)
{
	if (full)
	{
		throw std::exception("buffer is full");
	}
	buf[head] = val;
	head = ((head + 1) % max_size);
	full = head == tail;
}

net::CircularBuffer<char>& net::operator<<(net::CircularBuffer<char>& b, char val)
{
	b.Write(val);
	return b;
}
