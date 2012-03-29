#ifndef FLAT_NODE_CONTAINER_H
#define FLAT_NODE_CONTAINER_H

#include "../Node.hpp"
#include "NodeStructureInfoContainer.hpp"
#include <vector>

using namespace std;


class FlatNodeContainer {

    public:
        FlatNodeContainer(const vector<Node>* nodes);
		~FlatNodeContainer( void );
        
		NodeStructureInfoContainer* getNodeStructureInfoContainer( void );
		PackedNode* getPackedContainer(int* size);

	private:

		vector<Node> _nodes;

		int _width;
		int _height;
		int _left;
		int _bottom;
};


#endif