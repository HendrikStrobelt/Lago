#include "Connection.hpp"
#include <string>
#include <map>
#include <sstream>
#include "../context/Context.hpp"
#include <iostream>
#include "../GlobalConstants.hpp"

using namespace std;

Connection::Connection( void ) :
	_con("127.0.0.1", "4224")
{
	connect();
}


Connection::~Connection( void ) {
	_con.disconnect();
	_con.receiveString(); //wait for last words then terminate
}


void Connection::connect( void ) {
	if (_con.isConnectionAlive()) {
		cout << "already connected " << "\n";
	} else {
		_con.connect2Server();
		if (!_con.isConnectionAlive()) {
			cout << "oops couldn't connect to the server" << "\n";
		}	
	}
}


bool Connection::sync( void ) {
	if (_con.isConnectionAlive()) {
		float sideLength = context::_pixelSize * pow(SIDE_BASE, context::_sideExponent);
		
		if (context::_options._changedLocal) {
			//parameter change on this side => push
			stringstream s;
			s << "push# pixelSize$" << context::_pixelSize << " sideLength$" << sideLength;
			s << context::_options.toCommandString();

			_con.sendString(s.str());
			context::_options._changedLocal = false;
		} else {
			//get new option values => pull
			stringstream s;
			s << "pull# pixelSize$" << context::_pixelSize << " sideLength$" << sideLength;
			_con.sendString(s.str());
		
			if (_con.isConnectionAlive()) {
				string rec = _con.receiveString();

				string cmd = extractCommand(rec);
			
				if (cmd == "push") {
					//only correct possibility
					map<string, string> dataMap = extractData(rec);
					context::_options.update(dataMap);
					context::_scaleOptions[0].update(dataMap, "N.");
					context::_scaleOptions[1].update(dataMap, "E.");
				}
			}
		}

	}



	return _con.isConnectionAlive();
}


string Connection::extractCommand(string input) {
	int cmdEndIndex = input.find_first_of('#');
	string cmd = input.substr(0, cmdEndIndex);

	return cmd;
}



map<string, string> Connection::extractData(string input) {
	map<string, string> retMap;

	//remove the command
	int cmdEndIndex = input.find_first_of('#');
	string data = input.substr(cmdEndIndex + 1, input.length() - cmdEndIndex - 1);

	char* cstr = new char[data.length() + 1];
	strcpy(cstr, data.c_str());

	char* token;
	char* parameter;
	char* value;
	token = strtok(cstr, " $");

	while ( token != NULL) {

		parameter = new char[strlen(token) + 1];
		strcpy(parameter, token);
		token = strtok(NULL, " $");
		value = new char[strlen(token) + 1];
		strcpy(value, token);
		token = strtok(NULL, " $");

		retMap[parameter] = value;

		delete value;
		delete parameter;
	}

	delete cstr;

	return retMap;
}