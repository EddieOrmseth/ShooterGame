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

#include "Array.hpp"

class BasicConnection {

public:

	BasicConnection(SOCKET socket);
	~BasicConnection();

	SOCKET* getSocket();

	int sendData(const char* data, int length);
	int receiveData(char* data, int length);

	template <class T>
	int sendData(Array<T>* array);

	template <class T>
	int receiveData(Array<T>* array);

	//void setSendBufferBlocking(bool blocking);
	//void setReceiveBufferBlocking(bool blocking);
	int setBlocking(bool blocking);

	//bool isConnected();
	int shutdown();

private:

	SOCKET socket;
	//bool connected;

};

template <class T>
int BasicConnection::sendData(Array<T>* array) {
	return sendData((char*) array->getData(), array->size() * sizeof(T));
}

template <class T>
int BasicConnection::receiveData(Array<T>* array) {
	return receiveData((char*) array->getData(), array->size() * sizeof(T));
}

