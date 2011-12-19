#ifndef LABEL_H
#define LABEL_H

#include <string>
using namespace std;

struct Label {
	float x;
	float y;
	float weight;
	string text;

	Label( void ) : weight(1.0f) {};
};

#endif