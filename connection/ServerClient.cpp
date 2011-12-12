#include "ServerClient.hpp"
#include <iostream>

#define DEFAULT_BUFLEN 512

namespace ServerClient {


	//private methods
	int sendByteData(SOCKET socket, char* data, int length);
	int receiveByteData(SOCKET socket, char* recBuffer);


	//requests version 2.2 of WinSock
	//returns true if WinSock 2.2 is available
	bool initWinSock( void ) {
		WSADATA wsaData;
	
		int iResult;

		// Initialize Winsock
		iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
		if (iResult != 0) {
			cerr << "WSAStartup failed: " << iResult << "\n";
			return false;
		}

		return true;
	}


	//creates a standard ipv4 tcpip socket 
	//returns the created Socket and an stores the addrinfo setup in the
	//variable pointed to by addr_ptr
	//INVALID_SOCKET is returned to indicate an error
	SOCKET createSocket(char* host, char* port, addrinfo** addr_ptr) {
	
		struct addrinfo hints;
		
		ZeroMemory(&hints, sizeof (hints));
		hints.ai_family = AF_INET;				//IPV4
		hints.ai_socktype = SOCK_STREAM;		//a stream socket
		hints.ai_protocol = IPPROTO_TCP;		//TCPIP protocoll
		hints.ai_flags = AI_PASSIVE;

		// Resolve the local address and port to be used by the server
		int iResult = getaddrinfo(host, port, &hints, addr_ptr);

		if (iResult != 0) {
			cerr << "getaddrinfo failed: " << iResult << "\n";
			WSACleanup();
			return INVALID_SOCKET;
		}


		SOCKET ListenSocket = INVALID_SOCKET;
		
		// Create a SOCKET for the server to listen for client connections
		ListenSocket = socket((*addr_ptr)->ai_family, (*addr_ptr)->ai_socktype, (*addr_ptr)->ai_protocol);

		if (ListenSocket == INVALID_SOCKET) {
			cerr << "Error at socket(): " << WSAGetLastError() << "\n";
			freeaddrinfo(*addr_ptr);
			WSACleanup();
			return INVALID_SOCKET;
		}

		return ListenSocket;
	}


	//close the in going and out going connection
	//shut down the socket
	void closeAll(SOCKET socket) {
		if (socket != INVALID_SOCKET) {
			closesocket(socket);
		}
		WSACleanup();
	}

	//close the out going connection and indicate
	//that no further messages will be send
	//
	//return   1 => everything ok
	//           => error occured full connection closed
	int closeOutgoingConnection(SOCKET socket) {
		int success = 1;

		int iResult = shutdown(socket, SD_SEND);
		if (iResult == SOCKET_ERROR) {
			success = iResult;
			cerr << "shutdown failed: " <<  WSAGetLastError() << "\n";
			closeAll(socket);
		}

		return success;
	}


	//receives a \n terminated char sequence from the socket
	//and stores it into the string
	//
	//return > 0 => everything ok
	//         0 => other side closed send part of the connection
	//				nothing to receive any more
	//       < 0 => error occured connection closed
	int receiveString(SOCKET socket, string* text) {
		char recBuffer[DEFAULT_BUFLEN];
		int iResult;
		int success = 1;

		bool run = true;
		do {
			iResult = receiveByteData(socket, recBuffer);
			if (iResult > 0) {
				//reading data
				int read;

				if ((iResult == DEFAULT_BUFLEN) && (recBuffer[DEFAULT_BUFLEN-1] != '\n')) {
					//more data incomeing
					read = DEFAULT_BUFLEN;
				} else {
					read = iResult - 1; //omit \n
					
					//check last char for \n
					if (recBuffer[read] != '\n') {
						success = -10423234;
						closeAll(socket);
					}

					run = false; //terminate
				}
				
				char* cstr = new char[read + 1];
				memcpy(cstr, recBuffer, read);
				cstr[read] = '\0'; //terminate

				text->append(cstr);

				delete cstr;
			}

			if (iResult == 0) {
				//connection closed
				run = false;
			}
		} while (run == true);

		if (iResult <= 0) {
			success = iResult;
		}

		return success;
	}

	//sends the specified string to the socket as \n terminated
	//sequence of chars.
	//
	//return > 0 => everything ok
	//       < 0 => error occured connection closed
	int sendString(SOCKET socket, string text) {

		int success = 1;
		char* cstr = new char[text.size() + 1];
		strcpy(cstr, text.c_str());
		cstr[text.size()] = '\n';
		

		int iResult = sendByteData(socket, cstr, text.size() + 1);

		if (iResult <= 0) {
			success = iResult;
		}

		delete cstr;
		return success;
	}






	
	//receive data via the socket
	//the socket has to be completely initialized first
	//returns number of bytes received or an error code
	//data is stored to recBuffer
	int receiveByteData(SOCKET socket, char* recBuffer) {
	 
		int iResult = recv(socket, recBuffer, DEFAULT_BUFLEN, 0);
		  
		if (iResult > 0) {
			//cout << "Bytes received: " << iResult << "\n";
		} else if (iResult == 0) {
	   		cout << "Connection closed" << "\n";
		} else {
			//close connection
	  		cerr << "recv failed: " << WSAGetLastError() << "\n";
			closeAll(socket);
		}

		return iResult;
	}

	//send length bytes of data from data via the socket.
	//the socket has to be completely initialized first
	//returns number of bytes send or an error code
	int sendByteData(SOCKET socket, char* data, int length) {

		int iResult = send(socket, data, length, 0);
	  
		if (iResult == SOCKET_ERROR) {
			cerr << "send failed: " << WSAGetLastError() << "\n";
			closeAll(socket);
		} else {
			//cout << "Bytes Sent: " << iResult << "\n";
		}
	
		return iResult;
	}



}