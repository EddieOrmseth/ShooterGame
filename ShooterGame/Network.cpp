#include "Network.hpp"
#include "BasicConnection.hpp"

bool Network::initialized = false;
bool Network::wsaStartupComplete = false;

BasicConnection* Network::WaitForConnection(int family, int socketType, IPPROTO ipProtocol, int flags, std::string port) {
	if (!isInitialized()) {
        std::cout << "Failed to Initialize Network" << std::endl;
        return nullptr;
	}

    struct addrinfo* result = NULL;
    struct addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = family;
    hints.ai_socktype = socketType;
    hints.ai_protocol = ipProtocol;
    hints.ai_flags = flags;

    // Resolve the server address and port
    int getAddressResult = getaddrinfo(NULL, port.c_str(), &hints, &result);
    if (getAddressResult != 0) {
        printf("getaddrinfo failed with error: %d\n", getAddressResult);
        return nullptr;
    }

    // Create a SOCKET for connecting to server
    SOCKET ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        return nullptr;
    }

    // Setup the TCP listening socket
    int bindResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (bindResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        return nullptr;
    }

    freeaddrinfo(result);

    int listenResult = listen(ListenSocket, SOMAXCONN);
    if (listenResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        return nullptr;
    }

    // Accept a client socket
    SOCKET ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        return nullptr;
    }

    // No longer need server socket
    closesocket(ListenSocket);

    std::cout << "Connection Succsessful!" << std::endl;

    return new BasicConnection(ClientSocket);
}

BasicConnection* Network::TransmissionConnection(int family, int socketType, IPPROTO ipProtocol, int flags, std::string port, std::string serverName) {
    if (!isInitialized()) {
        std::cout << "Failed to Initialize Network" << std::endl;
        return nullptr;
    }

    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo* result = NULL;
    struct addrinfo* ptr = NULL;
    struct addrinfo hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = family;
    hints.ai_socktype = socketType;
    hints.ai_protocol = ipProtocol;
    hints.ai_flags = flags;

    // Resolve the server address and port
    int getAddressInfoResult = getaddrinfo(serverName.c_str(), port.c_str(), &hints, &result);
    if (getAddressInfoResult != 0) {
        printf("getaddrinfo failed with error: %d\n", getAddressInfoResult);
        return nullptr;
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            return nullptr;
        }

        // Connect to server.
        int connectResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (connectResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    std::cout << "Connection Succsessful!" << std::endl;

    return new BasicConnection(ConnectSocket);
}

bool Network::isInitialized() {
    if (initialized) {
        return true;
    }
    else {
        InitializeNetwork();
        if (initialized) {
            return true;
        }
        else {
            return false;
        }
    }
}

int Network::InitializeNetwork() {

    int wsaStartupStatus = 0;
    if (!wsaStartupComplete) {
        wsaStartupStatus = StartWSA();
    }


    if (wsaStartupStatus == 0) {
        initialized = true;
        return 0;
    }
    else {
        return 1;
    }
}

int Network::StartWSA() {
    WSADATA wsaData;
    // Initialize Winsock
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cout << "WSAStartup failed with error: " << iResult << std::endl;
    }
    else {
        wsaStartupComplete = true;
    }

    return iResult;
}

int Network::CloseNetwork() {

    int wsaCloseResult = 0;
    if (wsaStartupComplete) {
        wsaCloseResult = WSACleanup();
    }


    if (wsaCloseResult == 0) {
        initialized = false;
        return 0;
    }
    else {
        return 1;
    }
}

int Network::CloseWSA() {
    WSADATA wsaData;
    // Initialize Winsock
    int iResult = WSACleanup();
    if (iResult != 0) {
        std::cout << "WSACleanup failed with error: " << iResult << std::endl;
    }
    else {
        wsaStartupComplete = false;
    }

    return iResult;
}