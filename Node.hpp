#ifndef NODE_H
#define NODE_H

struct Node {
	float x;
	float y;
	short depth;
	float weight;
	Node( void ) : x(0) {};
};


#endif