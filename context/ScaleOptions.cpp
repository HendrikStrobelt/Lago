#include "ScaleOptions.hpp"
#include <sstream>
#include <iostream>
#include "Context.hpp"

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

		bool change = false;

		if ((it = dataMap.find(prefix + "mode")) != dataMap.end()) {

		//data exists
		if (it->second.compare("exp") == 0) {
			//exponential mode
			if (_linearMode) {
				change = true;
			}
			_linearMode = false;
	
			if ((it = dataMap.find(prefix + "exp")) != dataMap.end()) {
				float tmp = atof(it->second.c_str());
				if (_exponent != tmp) {
					change = true;
					_exponent = tmp;
				}
			}
		} else
		if (it->second.compare("lin") == 0) {
			//linear mode

			if (!_linearMode) {
				change = true;
			}
			_linearMode = true;

			if ((it = dataMap.find(prefix + "p1x")) != dataMap.end()) {	
				float tmp = atof(it->second.c_str());
				if (_controlPoints[0][0] != tmp) {
					change = true;
					_controlPoints[0][0] = tmp;
				}
			}
			if ((it = dataMap.find(prefix + "p2x")) != dataMap.end()) {		
				float tmp = atof(it->second.c_str());
				if (_controlPoints[1][0] != tmp) {
					change = true;
					_controlPoints[1][0] = tmp;
				}
			}
			if ((it = dataMap.find(prefix + "p3x")) != dataMap.end()) {					
				float tmp = atof(it->second.c_str());
				if (_controlPoints[2][0] != tmp) {
					change = true;
					_controlPoints[2][0] = tmp;
				}
			}
			if ((it = dataMap.find(prefix + "p4x")) != dataMap.end()) {					
				float tmp = atof(it->second.c_str());
				if (_controlPoints[2][0] != tmp) {
					change = true;
					_controlPoints[2][0] = tmp;
				}
			}
			if ((it = dataMap.find(prefix + "p1y")) != dataMap.end()) {					
				float tmp = atof(it->second.c_str());
				if (_controlPoints[0][1] != tmp) {
					change = true;
					_controlPoints[0][1] = tmp;
				}
			}
			if ((it = dataMap.find(prefix + "p2y")) != dataMap.end()) {					
				float tmp = atof(it->second.c_str());
				if (_controlPoints[1][1] != tmp) {
					change = true;
					_controlPoints[1][1] = tmp;
				}
			}
			if ((it = dataMap.find(prefix + "p3y")) != dataMap.end()) {					
				float tmp = atof(it->second.c_str());
				if (_controlPoints[2][1] != tmp) {
					change = true;
					_controlPoints[2][1] = tmp;
				}
			}
			if ((it = dataMap.find(prefix + "p4y")) != dataMap.end()) {					
				float tmp = atof(it->second.c_str());
				if (_controlPoints[3][1] != tmp) {
					change = true;
					_controlPoints[3][1] = tmp;
				}
			}
		}

		if (change) {
			context::visParameterChange();
		}
	}
}