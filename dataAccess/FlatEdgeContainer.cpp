#include "FlatEdgeContainer.hpp"
#include "../GlobalConstants.hpp"
#include <algorithm>


FlatEdgeContainer::FlatEdgeContainer(const vector<Edge>* edges) {
	for (int i = 0; i < edges->size(); i++) {
		_edgeVector.push_back(edges->at(i));
	}
}

FlatEdgeContainer::~FlatEdgeContainer( void ) {

}


//public

EdgeStructureInfoContainer* FlatEdgeContainer::getEdgeStructureInfoContainer( void ) {
	vector<int> allEdges;
	allEdges.push_back(_edgeVector.size());

	return new EdgeStructureInfoContainer(0, allEdges, allEdges);
}


PackedEdge* FlatEdgeContainer::getPackedContainer(int* size) {
	int arraySize = _edgeVector.size();
	PackedEdge* pe = new PackedEdge[arraySize];
	
	random_shuffle(_edgeVector.begin(), _edgeVector.end());

	for (int j = 0; j < _edgeVector.size(); j++) {
		pe[j] = _edgeVector.at(j);
	}

	*size = arraySize;
	return pe;
}

