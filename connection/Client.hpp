#ifndef CLIENT_H
#define CLIENT_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include "ServerClient.hpp"

using namespace std;

//Client class
class Client {

	public:
		Client(char* host, char* port);
		~Client( void );

		bool connect2Server( void );
		void disconnect( void );
		void sendString(string text);
		string receiveString( void );

		bool isConnectionAlive( void );

	private:
		bool connectSocket(SOCKET server, addrinfo* addr_ptr);

		SOCKET _serverConn;
		bool _connectionAlive;
		char* _port;
		char* _host;
};

#endif