#ifndef EDGE_H
#define EDGE_H

#include "Node.hpp"

struct Edge {
	float x1;
	float y1;
	float x2;
	float y2;
	short depth;
	float weight;
	Edge(Node* n1, Node* n2, short pdepth, float pweight) : x1(n1->x), y1(n1->y), x2(n2->x), y2(n2->y), depth(pdepth), weight(pweight) {};
};


#endif