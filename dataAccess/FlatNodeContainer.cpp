#include "FlatNodeContainer.hpp"
#include "../GlobalConstants.hpp"
#include <algorithm>

FlatNodeContainer::FlatNodeContainer(const vector<Node>* nodes) {

	//define borders
	float minX = MAX_COORD;
	float maxX = MIN_COORD;
	float minY = MAX_COORD;
	float maxY = MIN_COORD;

	for (int i = 0; i < nodes->size(); i++) {
		minX = min(nodes->at(i).x, minX);
		maxX = max(nodes->at(i).x, maxX);
		minY = min(nodes->at(i).y, minY);
		maxY = max(nodes->at(i).y, maxY);

		_nodes.push_back(nodes->at(i));
	}

	_width = maxX - minX;
	_height = maxY - minY;
	_left = minX;
	_bottom = minY;
		
}

FlatNodeContainer::~FlatNodeContainer( void ) {

}


//public methods


NodeStructureInfoContainer* FlatNodeContainer::getNodeStructureInfoContainer( void ) {
	vector<int> allNodes;
	allNodes.push_back(_nodes.size());
	
	return new NodeStructureInfoContainer(0, _width, _height, _left, _bottom, allNodes, allNodes);
}

PackedNode* FlatNodeContainer::getPackedContainer(int* size) {
	int arraySize = _nodes.size();
	PackedNode* pn = new PackedNode[arraySize];
	
	random_shuffle(_nodes.begin(), _nodes.end());
	for (int i = 0; i < _nodes.size(); i++) {
		pn[i] = _nodes[i];
	}

	*size = arraySize;
	return pn;
}
