#include "EdgeStructureInfoContainer.hpp"	
#include "../helper/Dumping.hpp"
#include "../GlobalConstants.hpp"

EdgeStructureInfoContainer::EdgeStructureInfoContainer(int maxDepth, vector<int> leafNodes, vector<int> allNodes) {
	_maxDepth = maxDepth;
	_leafNodes = leafNodes;
	_allNodes = allNodes;
}
		
EdgeStructureInfoContainer::~EdgeStructureInfoContainer( void ) {

}
		
//public


int EdgeStructureInfoContainer::getMaxDepth( void ) const {
	return _maxDepth;
}

int EdgeStructureInfoContainer::getRenderEdges(int depth) const {
	if (EDGE_HIERARCHY_FLAT) {
		return _allNodes[0];
	} else {
		int leafs = 0;
		for (int i = 0; i < depth; i++) {
			leafs += _leafNodes[i];
		}
		return leafs + _allNodes[depth];
	}
}


int EdgeStructureInfoContainer::getAllEdges(int depth) const {
	if (EDGE_HIERARCHY_FLAT) {
		return _allNodes[0];
	} else {
		int edges = 0;
		for (int i = 0; i <= depth; i++) {
			edges += _allNodes[i];
		}
		return edges;
	}
}



//dump to disc

void EdgeStructureInfoContainer::serialize(ofstream* dataStream) {
	dump::w(dataStream, _maxDepth);
	dump::w(dataStream, &_leafNodes);
	dump::w(dataStream, &_allNodes);
}

EdgeStructureInfoContainer::EdgeStructureInfoContainer(ifstream* dataStream) {
	dump::r(dataStream, &_maxDepth);
	dump::r(dataStream, &_leafNodes);
	dump::r(dataStream, &_allNodes);
}
