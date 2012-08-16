#ifndef LABEL_H
#define LABEL_H

#include "GlobalConstants.hpp"
#include <string>
#include <map>
#include <sstream>

using namespace std;

#define MAX_LABEL_LENGTH 30

struct Label {
	float x;
	float y;
	float weight;
	char text[MAX_LABEL_LENGTH + 1]; //+1 for /0
	int id;

	Label( void ) : weight(1.0f) {};

	
	//network constructor
	Label(map<string, string> dataMap) {
			map<string, string>::iterator it;
			if ((it = dataMap.find("TEXT")) != dataMap.end()) {
				stringstream(it->second) >> text;
			}
			
			weight = 1.0f;
	}
};

#endif