#ifndef EDGE_HIERARCHY_H
#define EDGE_HIERARCHY_H

#include "QuadTree.hpp"
#include "EdgeStructureInfoContainer.hpp"
#include "..\Edge.hpp"

#include <vector>

using namespace std;

class EdgeHierarchy {

	public:
		EdgeHierarchy(const vector<Edge>* edges, const QuadTree* quadTree);
		~EdgeHierarchy( void );

		EdgeStructureInfoContainer* getEdgeStructureInfoContainer( void );
		PackedEdge* getPackedHierarchy(int* size);

	private:

		//creation helper methods
		void createHierarchy( void );
		void divideEdges(TravelIndex t1, TravelIndex t2, vector<int>* indices, float weightSum);
		void divideEdges(TravelIndex t1, TravelIndex t2, int index);

		void addEdge(Edge e, int level);//edge depths may be negative (inner edge vs leaf edge) therefore
										//level is given as insert point however  abs(edge.depth) == level


		int _maxDepth;
		vector<vector<Edge> *> _edgeVectors;
		vector<int> _leafNodes;

		//temporary pointer storage for recursive calls
		const vector<Edge>* _edges;
		const QuadTree* _quadTree;
};


#endif