#ifndef EDGE_H
#define EDGE_H

#include "Node.hpp"

struct ReferenceEdge {
	int n1Ref;
	int n2Ref;
	float weight;
	ReferenceEdge( void ) : weight(1.0f) {};
};

struct Edge {
	float x1;
	float y1;
	float x2;
	float y2;
	short depth;
	float weight;
	Edge( void ) : weight(1.0f) {};
	Edge(Node* n1, Node* n2, ReferenceEdge* refEdge) : x1(n1->x), y1(n1->y), x2(n2->x), y2(n2->y), weight(refEdge->weight) {};
	Edge(Node* n1, Node* n2, short pdepth, float pweight) : x1(n1->x), y1(n1->y), x2(n2->x), y2(n2->y), depth(pdepth), weight(pweight) {};
};

#pragma pack(push, 1)
	struct PackedEdge {
		float x1;
		float y1;
		float x2;
		float y2;
		short depth;
		float weight;
		PackedEdge( void ) : weight(1.0f) {};
		PackedEdge(Edge e) : x1(e.x1), y1(e.y1), x2(e.x2), y2(e.y2), depth(e.depth), weight(e.weight) {};
	};
#pragma pack(pop)

struct ProcessedEdge { //GPU ONLY
	float x1;
	float y1;
	float x2;
	float y2;
	int angel;
	float weight;
};

#endif