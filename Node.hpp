#ifndef NODE_H
#define NODE_H

struct Node {
	float x;
	float y;
	short depth;
	float weight;
	Node( void ) : weight(1.0f) {};
	Node(float px, float py, float pweight) : x(px), y(py), weight(pweight) {};
};


#endif