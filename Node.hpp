#ifndef NODE_H
#define NODE_H

#include <GL/glew.h>
#include <GL/glfw.h>
#include <map>
#include <sstream>

using namespace std;

struct Node {
	float x;
	float y;
	short depth;
	int labelID;
	float weight;
	Node( void ) : weight(1.0f), depth(0), labelID(-1) {};
	Node(float px, float py, float pweight) : x(px), y(py),  weight(1.0f), depth(0), labelID(-1) {};

	//network constructor
	Node(map<string, string> dataMap) :  weight(1.0f), depth(0), labelID(-1){
			map<string, string>::iterator it;
			if ((it = dataMap.find("X")) != dataMap.end()) {
				stringstream(it->second) >> x;
			}

			if ((it = dataMap.find("Y")) != dataMap.end()) {
				stringstream(it->second) >> y;
			}

			if ((it = dataMap.find("W")) != dataMap.end()) {
				stringstream(it->second) >> weight;
			}
	}
};


#pragma pack(push, 1)
	struct PackedNode {
		float x;
		float y;
		short depth;
		int labelID;
		float weight;
		PackedNode( void ) : weight(1.0f), labelID(-1) {};
		PackedNode(Node n) : x(n.x), y(n.y), depth(n.depth), weight(n.weight), labelID(n.labelID) {};
	};
#pragma pack(pop)

#endif