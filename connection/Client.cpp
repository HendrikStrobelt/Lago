#include "Client.hpp"
#include <iostream>

Client::Client(char* host, char* port) {
	_connectionAlive = false;
	_port = port;
	_host = host;
}


Client::~Client( void ) {
	ServerClient::closeAll(_serverConn);
}

//tries to connect to a running server
bool Client::connect2Server( void ) {
	if (!_connectionAlive) {
		bool init = ServerClient::initWinSock();
	
		if (init) {
			struct addrinfo* addr_ptr = NULL;
			_serverConn = ServerClient::createSocket(_host, _port, &addr_ptr);  
		
			if (_serverConn != INVALID_SOCKET) {
				bool activate = connectSocket(_serverConn, addr_ptr);
				freeaddrinfo(addr_ptr);

				if (activate) {
					//everything successful
					cout << "Client ready:" << "\n";
					_connectionAlive = true;
				}
			}
		}
	}
	return _connectionAlive;
}


bool Client::isConnectionAlive( void ) {
	return _connectionAlive;
}


void Client::disconnect( void ) {
	ServerClient::closeOutgoingConnection(_serverConn);
}
		

void Client::sendString(string text) {
	if (_connectionAlive) {
		int res  = ServerClient::sendString(_serverConn, text);
		if (res < 0) {
			_connectionAlive = false;
		}
	}
}


string Client::receiveString( void ) {
	string ret = "";
	if (_connectionAlive) {
		int res  = ServerClient::receiveString(_serverConn, &ret);
		if (res < 0) {
			_connectionAlive = false;
		} else if (res == 0) {
			//client indicates shutdown
			ServerClient::closeAll(_serverConn);
			_connectionAlive = false;
		}
	}
	return ret;
}




bool Client::connectSocket(SOCKET server, addrinfo* addr_ptr) {
	
	int iResult;
	// Connect to server.
	iResult = connect(server, addr_ptr->ai_addr, (int)addr_ptr->ai_addrlen);

	if (iResult == SOCKET_ERROR) {
		cerr << "error while connecting " << iResult << "\n";
		closesocket(server);
		WSACleanup();
		return false;
	}

	return true;
}