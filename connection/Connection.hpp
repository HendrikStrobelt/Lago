#ifndef CONNECTION_H
#define CONNECTION_H

#include "Server.hpp"
#include <string>
#include <vector>
#include <map>

class Connection {

	public:
		Connection( void );
		~Connection( void );

		bool isConnected();
		bool sync( void );
		void waitForClient( void );
		void loadData( void );

	private:
		map<string, string> extractData(string input);
		string extractCommand(string input);
		
		Server _server;

};

#endif