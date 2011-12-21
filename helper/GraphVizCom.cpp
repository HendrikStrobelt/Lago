#include "GraphVizCom.hpp"
#include <string>
#include <sstream>
#include <iostream>
#include <iterator>

#include "execStream\exec-stream.h"
#include "../context/Context.hpp"


using namespace std;

namespace graphVizCom {

	//private
	vector<string> split(string line);

	stringstream _commands;
	int _nr;
	int _windowWidth;
	int _windowHeight;

	//public
	void prepare( void ) {
		context::getWindowSize(&_windowWidth, &_windowHeight);

		_nr = -2;
		_commands.str("");

		cout << _commands;
		_commands << "graph{ \n";
		_commands << "graph [overlap=\"prism\" overlap_scaling=\"0\" sep=\"+0\"] \n";

		//spanning points far away to be sure they don't disturb
		add(-_windowWidth,-_windowHeight,1,1);
		add(_windowWidth*2, _windowHeight*2, 1, 1);
	}

	void add(int centerX, int centerY, int width, int height) {
		if (_nr < 0) {
			_commands << "sp" << abs(_nr);
		} else {
			_commands << _nr;
		}		
		_commands << " [pos=\"" << centerX*72.0f << "," << centerY*72.0f << "\", ";
		_commands << "width=\"" << ((float)width) << "\", ";
		_commands << "height=\"" << ((float)height) << "\" ";
		_commands << "shape=\"box\" fixedsize=\"true\"] \n";
		_nr++;
	}


	vector<MovedBox>* transmit( void ) {
		vector<MovedBox>* ret = new vector<MovedBox>();

		try {
			exec_stream_t es("C:\\Program Files (x86)\\Graphviz 2.28\\bin\\neato.exe", "-Tplain -n" );

			_commands << "} \n";
			es.in() << _commands.str();
			cout << _commands.str();
			es.close_in();

			MovedBox tmp;
			string line;
			while(getline(es.out(), line).good() ) {
				if (line.length() > 4 && line.substr(0, 4) == "node") {
					vector<string> tokens = split(line);
					
					if (!(tokens[1] == "sp1" || tokens[1] == "sp2")) { //ignore spanning points
						tmp.id = ((int)(floor(atof(tokens[1].c_str()))));
						tmp.x = ((int)(floor(atof(tokens[2].c_str())))) - _windowWidth;//correct spanning points
						tmp.y = ((int)(floor(atof(tokens[3].c_str())))) - _windowHeight; 

						ret->push_back(tmp);
					}
				}
			}
			es.close();
		} catch( std::exception const & e ) {
		    std::cerr << "error: "  <<  e.what()  <<  "\n";
		}

		return ret;
	}

	vector<string> split(string line) {
		 istringstream iss(line);
		 
		 vector<string> tokens;
		 copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(tokens));
		 return tokens;
	}
};