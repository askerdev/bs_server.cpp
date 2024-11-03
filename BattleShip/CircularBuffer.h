#pragma once
#include "Packet.h"
#include <memory>
#include <queue>
#include <vector>

namespace net
{
	template <typename T>
	class CircularBuffer
	{
	public:
		CircularBuffer(const int&);
		T Read();
		void Write(T);
		void Erase();
		bool IsFull();
		friend CircularBuffer<T>& operator<<(CircularBuffer<T>&, T);
	private:
		bool full;
		int max_size;
		int head;
		int tail;
		std::unique_ptr<T[]> buf;
	};
}