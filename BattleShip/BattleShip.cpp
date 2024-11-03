#pragma comment(lib, "ws2_32.lib")
#include "Application.h"
#include <iostream>
#include <winsock2.h>

int main()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	try
	{
		Application app;
		app.Main();
	} catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	WSACleanup();
	return 0;
}