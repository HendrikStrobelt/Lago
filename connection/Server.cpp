#include "Server.hpp"
#include <iostream>
#include "..\helper\Dumping.hpp"



Server::Server(char* port) {
	_connectionAlive = false;
	_port = port;
}

Server::~Server( void ) {
	ServerClient::closeAll(_clientConn);
	ServerClient::closeAll(_server);
}

//starts the server and waits for the single client
bool Server::start( void ) {
	if (!_connectionAlive) {
		bool init = ServerClient::initWinSock();
		if (init) {
			struct addrinfo* addr_ptr = NULL;
		    _server = ServerClient::createSocket(NULL, _port, &addr_ptr);  
		
			if (_server != INVALID_SOCKET) {
			    bool activate = bindAndListen(_server, addr_ptr);
			    freeaddrinfo(addr_ptr);

				if (activate) {
					//everything successful
					cout << "Server ready and waiting..." << "\n";
					_clientConn = accept(_server, NULL, NULL);

					if (_clientConn != INVALID_SOCKET) {
						cout << "Connection established:" << "\n";
						_connectionAlive = true;
					}
				}
			}
		}
	}
	return _connectionAlive;
}


bool Server::isConnectionAlive( void ) {
	return _connectionAlive;
}

void Server::disconnect( void ) {
	ServerClient::closeOutgoingConnection(_clientConn);
}

void Server::sendString(string text) {
	if (_connectionAlive) {
		int res  = ServerClient::sendString(_clientConn, text);
		if (res < 0) {
			ServerClient::closeAll(_clientConn);
			_connectionAlive = false;
		}
	}
}


string Server::receiveString( void ) {
	string ret = "";
	if (_connectionAlive) {
		int res  = ServerClient::receiveString(_clientConn, &ret);
		if (res < 0) {
			_connectionAlive = false;
		} else if (res == 0) {
			//client indicates shutdown
			ServerClient::closeAll(_clientConn);
			_connectionAlive = false;
		}
	}
	return ret;
}

bool Server::receiveNodes(int nrNodeBytes) {
	if (_connectionAlive) {

		//node file
		char* data = new char[nrNodeBytes];
		ServerClient::receiveData(_clientConn, data, nrNodeBytes);

		string nodeFile = ("_tmpFiles//Nodes.out");
		ofstream nodes(nodeFile, ios::binary);
		dump::w(&nodes, data, nrNodeBytes);
		nodes.close();

		return true;
	} else {
		return false;
	}
}

bool Server::receiveEdges(int nrEdgeBytes) {
	if (_connectionAlive) {

		//node file
		char* data = new char[nrEdgeBytes];
		ServerClient::receiveData(_clientConn, data, nrEdgeBytes);

		string nodeFile = ("_tmpFiles//Edges.out");
		ofstream nodes(nodeFile, ios::binary);
		dump::w(&nodes, data, nrEdgeBytes);
		nodes.close();

		return true;
	} else {
		return false;
	}
}


//try to bind the server with the specified addr_ptr infos
//and initialize listening
bool Server::bindAndListen(SOCKET ListenSocket, addrinfo* addr_ptr) {
	int iResult;
	
	// bind the socket
    iResult = bind( ListenSocket, addr_ptr->ai_addr, (int)addr_ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        cerr << "bind failed with error: " << WSAGetLastError() << "\n";
        freeaddrinfo(addr_ptr);

		ServerClient::closeAll(ListenSocket);
        return false;
    }

	//listen for incoming connections
	if ( listen( ListenSocket, SOMAXCONN ) == SOCKET_ERROR ) {
		cerr <<  "Listen failed with error: " << WSAGetLastError() << "\n";

		ServerClient::closeAll(ListenSocket);
		return false;
    }

	return true;
}

//waits for a connection to accept it
SOCKET Server::acceptConnection(SOCKET ListenSocket) {
	SOCKET ClientSocket;
	ClientSocket = INVALID_SOCKET;
	
	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);

	if (ClientSocket == INVALID_SOCKET) {
		cerr << "accept failed: " << WSAGetLastError() << "\n";
		
		ServerClient::closeAll(ListenSocket);
		return INVALID_SOCKET;
	}

    return ClientSocket;
}