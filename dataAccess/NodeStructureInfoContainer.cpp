#include "NodeStructureInfoContainer.hpp"
#include "../helper/Dumping.hpp"
#include <algorithm>

NodeStructureInfoContainer::NodeStructureInfoContainer(int maxDepth, float width, float height, float left, float bottom, vector<int> leafNodes, vector<int> allNodes) {
	_maxDepth = maxDepth;
	_width = width;
	_height = height;
	_left = left;
	_bottom = bottom;

	_leafNodes = leafNodes;
	_allNodes = allNodes;
}

NodeStructureInfoContainer::~NodeStructureInfoContainer( void ) {

}

//Public

int NodeStructureInfoContainer::getMaxDepth( void ) const {
	return _maxDepth;
}

float NodeStructureInfoContainer::getWidth( void ) const {
	return _width;
}

float NodeStructureInfoContainer::getHeight( void ) const {
	return _height;
}

float NodeStructureInfoContainer::getLeftBorder( void ) const {
	return _left;
}

float NodeStructureInfoContainer::getBottomBorder( void ) const {
	return _bottom;
}

//non simple

int NodeStructureInfoContainer::getJoinDepth(float desiredMinDistance) const {
	float maxRectSide = std::max(_width, _height);
	float depth;

	if (desiredMinDistance == 0.0f) {
		depth = _maxDepth;
	} else {
		//select an appropriate level of the quad tree
		depth = 0;
		while (maxRectSide > desiredMinDistance && depth < _maxDepth) {
			depth++;
			maxRectSide /= 2.0f;
		}
	}

	return depth;
}

int NodeStructureInfoContainer::getRenderNodes(int depth) const {
	int leafs = 0;
	for (int i = 0; i < depth; i++) {
		leafs += _leafNodes[i];
	}

	return leafs + _allNodes[depth];
}

int NodeStructureInfoContainer::getAllNodes(int depth) const {
	int nodes = 0;
	for (int i = 0; i <= depth; i++) {
		nodes += _allNodes[i];
	}
	return nodes;
}


//DUMP TO DISC

void NodeStructureInfoContainer::serialize(ofstream* dataStream) {
	dump::w(dataStream, _maxDepth);
	dump::w(dataStream, _width);
	dump::w(dataStream, _height);
	dump::w(dataStream, _left);
	dump::w(dataStream, _bottom);
	dump::w(dataStream, &_leafNodes);
	dump::w(dataStream, &_allNodes);
}

NodeStructureInfoContainer::NodeStructureInfoContainer(ifstream* dataStream) {
	dump::r(dataStream, &_maxDepth);
	dump::r(dataStream, &_width);
	dump::r(dataStream, &_height);
	dump::r(dataStream, &_left);
	dump::r(dataStream, &_bottom);
	dump::r(dataStream, &_leafNodes);
	dump::r(dataStream, &_allNodes);
}