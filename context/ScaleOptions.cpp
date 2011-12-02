#include "ScaleOptions.hpp"
#include <sstream>
#include <iostream>

ScaleOptions::ScaleOptions( void ) {
	_linearMode = true;
	_exponent = 1.0f;
	_controlPoints[0][0] = 0.0f;
	_controlPoints[0][1] = 0.0f;
	_controlPoints[1][0] = 0.0f;
	_controlPoints[1][1] = 0.0f;
	_controlPoints[2][0] = 0.8f;
	_controlPoints[2][1] = 1.0f;
	_controlPoints[3][0] = 1.0f;
	_controlPoints[3][1] = 1.0f;
}

ScaleOptions::~ScaleOptions( void ) {
	
}

void ScaleOptions::update(map<string, string> dataMap, string prefix) {
		map<string, string>::iterator it;

		if ((it = dataMap.find(prefix + "mode")) != dataMap.end()) {

		//data exists
		if (it->second.compare("exp") == 0) {
			//exponential mode
			_linearMode = false;
	
			if ((it = dataMap.find(prefix + "exp")) != dataMap.end()) {
				_exponent = atof(it->second.c_str());
			}
		} else
		if (it->second.compare("lin") == 0) {
			//linear mode

			_linearMode = true;

			if ((it = dataMap.find(prefix + "p1x")) != dataMap.end()) {					
				_controlPoints[0][0] = atof(it->second.c_str());
			}
			if ((it = dataMap.find(prefix + "p2x")) != dataMap.end()) {					
				_controlPoints[1][0] = atof(it->second.c_str());
			}
			if ((it = dataMap.find(prefix + "p3x")) != dataMap.end()) {					
				_controlPoints[2][0] = atof(it->second.c_str());
			}
			if ((it = dataMap.find(prefix + "p4x")) != dataMap.end()) {					
				_controlPoints[3][0] = atof(it->second.c_str());
			}
			if ((it = dataMap.find(prefix + "p1y")) != dataMap.end()) {					
				_controlPoints[0][1] = atof(it->second.c_str());
			}
			if ((it = dataMap.find(prefix + "p2y")) != dataMap.end()) {					
				_controlPoints[1][1] = atof(it->second.c_str());
			}
			if ((it = dataMap.find(prefix + "p3y")) != dataMap.end()) {					
				_controlPoints[2][1] = atof(it->second.c_str());
			}
			if ((it = dataMap.find(prefix + "p4y")) != dataMap.end()) {					
				_controlPoints[3][1] = atof(it->second.c_str());
			}
		}
	}
}