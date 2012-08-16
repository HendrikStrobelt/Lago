#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include "ServerClient.hpp"

using namespace std;

//Singel Connection Server Class
class Server {

	public:
		Server(char* port);
		~Server( void );

		bool start( void );
		void disconnect( void );
		void sendString(string text);
		string receiveString( void );

		bool isConnectionAlive( void );

	private:
		bool bindAndListen(SOCKET ListenSocket, addrinfo* addr_ptr);
		SOCKET acceptConnection(SOCKET ListenSocket);

		SOCKET _clientConn;
		SOCKET _server;
		bool _connectionAlive;
		char* _port;
};

#endif