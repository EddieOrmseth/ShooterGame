#pragma once

#undef UNICODE
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#include <iostream>

class BasicConnection;

class Network {

public:

	// Add Overloads For This
	enum class IPFamily {

	};

	static BasicConnection* WaitForConnection(int family, int socketType, IPPROTO ipProtocol, int flags, std::string port);
	static BasicConnection* TransmissionConnection(int family, int socketType, IPPROTO ipProtocol, int flags, std::string port, std::string serverName);

	static int InitializeNetwork();
	static int CloseNetwork();

private:

	static bool isInitialized();

	static int StartWSA();
	static int CloseWSA();

	static bool initialized;
	static bool wsaStartupComplete;

};
