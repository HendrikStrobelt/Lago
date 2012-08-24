#include "Connection.hpp"
#include <string>
#include <map>
#include <sstream>
#include "../context/Context.hpp"
#include <iostream>
#include "../GlobalConstants.hpp"
#include "../Node.hpp"
#include "../Edge.hpp"
#include "../Label.hpp"

using namespace std;

Connection::Connection( void ) :
	_server("4224")
{
	
}


Connection::~Connection( void ) {
	_server.disconnect();
	_server.receiveString(); //wait for last words then terminate
}


void Connection::waitForClient( void ) {
	if (_server.isConnectionAlive()) {
		cout << "already connected " << "\n";
	} else {
		_server.start();
	}
}

void Connection::loadData( void ) {
	if (_server.isConnectionAlive()) {

		//start data transmission
		_server.sendString("getData#");
		string ans = _server.receiveString();


		//hook for shutdown client
		if (extractCommand(ans) == "exit") {
			_server.disconnect();
			context::_run = false;
		} else {

			if (extractCommand(ans) == "DataParameters") {
				//get parameters
				map<string, string>::iterator it;
				map<string, string> dataMap = extractData(ans);
				
				int nodeBytes = 0;
				int edgeBytes = 0;

				if ((it = dataMap.find("edgeBytes")) != dataMap.end()) {
					stringstream(it->second) >> edgeBytes;				
				}

				if ((it = dataMap.find("nodeBytes")) != dataMap.end()) {
					stringstream(it->second) >> nodeBytes;				
				}			


				bool nodeSucc = false;
				bool edgeSucc = false;

				//got all parameters start transmission
				_server.sendString("akk#");
				if (nodeBytes > 0) {
					nodeSucc = _server.receiveNodes(nodeBytes);
				}
				if (edgeBytes > 0) {
					_server.sendString("akk#");
					edgeSucc = _server.receiveEdges(edgeBytes);
				}

				_server.sendString("akk#");
				ans = _server.receiveString();
								
				//success
				if (extractCommand(ans) == "DataTransfered") {
					if (nodeSucc && edgeSucc) {
						context::setNewData("_tmpFiles//Nodes.out","_tmpFiles//Edges.out");
					} else 
					if (nodeSucc) {
						context::setNewData("_tmpFiles//Nodes.out");
					}
				}
			}		
		}
	}
}

bool Connection::sync( void ) {
	if (_server.isConnectionAlive()) {
		float sideLength = context::_pixelSize * pow(SIDE_BASE, context::_sideExponent);
		
		if (context::_options._changedLocal) {
			//parameter change on this side => push
			stringstream s;
			s << "push# pixelSize$" << context::_pixelSize << " sideLength$" << sideLength;
			s << context::_options.toCommandString();

			_server.sendString(s.str());
			context::_options._changedLocal = false;
		} else {
			//get new option values => pull
			stringstream s;
			s << "pull# pixelSize$" << context::_pixelSize << " sideLength$" << sideLength;
			_server.sendString(s.str());
		
			if (_server.isConnectionAlive()) {
				string rec = _server.receiveString();

				string cmd = extractCommand(rec);
			
				if (cmd == "push") {
					//only correct possibility
					map<string, string> dataMap = extractData(rec);
					context::_options.update(dataMap);
					context::_scaleOptions[0].update(dataMap, "N.");
					context::_scaleOptions[1].update(dataMap, "E.");
					context::_scaleOptions[2].update(dataMap, "L.");
				}
			}
		}

	}



	return _server.isConnectionAlive();
}


string Connection::extractCommand(string input) {
	int cmdEndIndex = input.find_first_of('#');
	string cmd = input.substr(0, cmdEndIndex);

	return cmd;
}


bool Connection::isConnected() {
	return _server.isConnectionAlive();
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