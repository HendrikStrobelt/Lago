#ifndef SERVER_CLIENT_H
#define SERVER_CLIENT_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

using namespace std;

namespace ServerClient {

	//general socket init methods
	bool initWinSock( void );
	SOCKET createSocket(char* host, char* port, addrinfo** addr_ptr);

	//data transmission methods
	int sendString(SOCKET socket, string text);
	int receiveString(SOCKET socket, string* text);

	//socket shutdown
	void closeAll(SOCKET socket);
	int closeOutgoingConnection(SOCKET socket);
};



#endif