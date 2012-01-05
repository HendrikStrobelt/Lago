#ifndef LABEL_H
#define LABEL_H

#include "GlobalConstants.hpp"
#include <string>
using namespace std;

#define MAX_LABEL_LENGTH 30

struct Label {
	float x;
	float y;
	float weight;
	char text[MAX_LABEL_LENGTH + 1]; //+1 for /0
	int id;

	Label( void ) : weight(1.0f) {};
};

#endif