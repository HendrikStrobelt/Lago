#ifndef CONNECTION_H
#define CONNECTION_H

#include "Client.hpp"
#include <string>
#include <vector>
#include <map>

class Connection {

	public:
		Connection( void );
		~Connection( void );

		bool sync( void );
		void Connection::connect( void ); //connects / reconnects to the server

	private:
		map<string, string> extractData(string input);
		string extractCommand(string input);
		
		Client _con;

};

#endif