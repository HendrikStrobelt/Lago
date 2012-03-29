#ifndef FLAT_EDGE_CONTAINER_H
#define FLAT_EDGE_CONTAINER_H

#include "EdgeStructureInfoContainer.hpp"
#include "..\Edge.hpp"

#include <vector>

using namespace std;

class FlatEdgeContainer {

	public:
		FlatEdgeContainer(const vector<Edge>* edges);
		~FlatEdgeContainer( void );

		EdgeStructureInfoContainer* getEdgeStructureInfoContainer( void );
		PackedEdge* getPackedContainer(int* size);

	private:

		vector<Edge> _edgeVector;
};


#endif