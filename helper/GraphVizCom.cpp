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

	float _centerX;
	float _centerY;

	//public
	void prepare( void ) {
		context::getWindowSize(&_windowWidth, &_windowHeight);

		_nr = 0;
		_centerX = 0.0f;
		_centerY = 0.0f;
		_commands.str("");

		cout << _commands;
		_commands << "graph{ \n";
		_commands << "graph [overlap=\"prism\" overlap_scaling=\"0\" sep=\"+0\"] \n";
	}

	void add(int centerX, int centerY, int width, int height) {
		_commands << _nr;
		_commands << " [pos=\"" << centerX*72.0f << "," << centerY*72.0f << "\", ";
		_commands << "width=\"" << ((float)width) << "\", ";
		_commands << "height=\"" << ((float)height) << "\" ";
		_commands << "shape=\"box\" fixedsize=\"true\"] \n";
		_nr++;
		_centerX += centerX;
		_centerY += centerY;
	}


	vector<MovedBox>* transmit( void ) {
		vector<MovedBox>* ret = new vector<MovedBox>();

		try {
			exec_stream_t es("C:\\Program Files (x86)\\Graphviz 2.28\\bin\\neato.exe", "-Tplain -n" );

			_commands << "} \n";
			es.in() << _commands.str();
			cout << _commands.str();
			es.close_in();
		
			float centerX2 = 0.0f;
			float centerY2 = 0.0f;

			MovedBox tmp;
			string line;
			while(getline(es.out(), line).good() ) {
				if (line.length() > 4 && line.substr(0, 4) == "node") {
					vector<string> tokens = split(line);
					
					tmp.id = ((int)(floor(atof(tokens[1].c_str()))));
					tmp.x = ((int)(floor(atof(tokens[2].c_str()))));
					tmp.y = ((int)(floor(atof(tokens[3].c_str()))));
					centerX2 += tmp.x;
					centerY2 += tmp.y;
					ret->push_back(tmp);
				}
			}
			es.close();

			_centerX /= _nr;
			_centerY /= _nr;
			centerX2 /= _nr;
			centerY2 /= _nr;

			float xMove = _centerX - centerX2;
			float yMove = _centerY - centerY2;

			for (int i = 0; i < ret->size(); i++) {
				ret->at(i).x = ret->at(i).x + xMove;
				ret->at(i).y = ret->at(i).y + yMove;
			}

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