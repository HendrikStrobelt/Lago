#ifndef NODE_H
#define NODE_H

#include <GL/glew.h>
#include <GL/glfw.h>

struct Node {
	float x;
	float y;
	short depth;
	int labelID;
	float weight;
	Node( void ) : weight(1.0f), labelID(-1), depth(0) {};
	Node(float px, float py, float pweight) : x(px), y(py), weight(pweight), labelID(-1), depth(0) {};
};


#pragma pack(push, 1)
	struct PackedNode {
		float x;
		float y;
		short depth;
		int labelID;
		float weight;
		PackedNode( void ) : weight(1.0f), labelID(-1), depth(0) {};
		PackedNode(Node n) : x(n.x), y(n.y), depth(n.depth), weight(n.weight), labelID(n.labelID) {};
	};
#pragma pack(pop)

#endif