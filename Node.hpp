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


#pragma pack(push, 1)
	struct PackedNode {
		float x;
		float y;
		short depth;
		float weight;
		PackedNode( void ) : weight(1.0f) {};
		PackedNode(Node n) : x(n.x), y(n.y), depth(n.depth), weight(n.weight) {};
	};
#pragma pack(pop)

#endif