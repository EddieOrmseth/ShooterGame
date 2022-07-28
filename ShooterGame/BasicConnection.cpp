#include "BasicConnection.hpp"

BasicConnection::BasicConnection(SOCKET socket) {
	this->socket = socket;
}

BasicConnection::~BasicConnection() {
	shutdown();
}

SOCKET* BasicConnection::getSocket() {
	return &socket;
}

int BasicConnection::sendData(const char* data, int length) {
	return send(socket, data, length, 0);
}

int BasicConnection::receiveData(char* data, int length) {
	return recv(socket, data, length, 0);
}

//void BasicConnection::setSendBufferBlocking(bool blocking) {
//	// TO DO
//	u_long arg = blocking ? 0 : 1;
//	ioctlsocket(socket, FIONBIO, &arg);
//}
//
//void BasicConnection::setReceiveBufferBlocking(bool blocking) {
//	// TO DO
//	u_long arg = blocking ? 0 : 1;
//	ioctlsocket(socket, FIONBIO, &arg);
//}

int BasicConnection::setBlocking(bool blocking) {
	u_long arg = blocking ? 0 : 1;
	return ioctlsocket(socket, FIONBIO, &arg);
}

//bool BasicConnection::isConnected() {
//	return connected;
//}

int BasicConnection::shutdown() {
	return closesocket(socket);
}