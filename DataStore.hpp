#ifndef DATA_CACHE_H
#define DATA_CACHE_H

#include <string>
#include "Node.hpp"
#include "Edge.hpp"
#include "Label.hpp"
#include "dataAccess\NodeStructureInfoContainer.hpp"
#include "dataAccess\EdgeStructureInfoContainer.hpp"

using namespace std;

class DataStore {
	
	public:
		DataStore( void );
		~DataStore( void );

		void setData(vector<Node>* nodes, vector<ReferenceEdge>* refEdges, vector<Label>* nodeLabels, bool withNodeWeights);

		const vector<Label>* getIndexedLabels( void );
		const PackedNode* getPackedNodes(int* size);
		const PackedEdge* getPackedEdges(int* size);
		const NodeStructureInfoContainer* getNodeStructureInfo( void );
		const EdgeStructureInfoContainer* getEdgeStructureInfo( void );

		const bool hasLabels( void );
		const bool hasEdges( void );

	private:
		void processData(vector<Node>* nodes, vector<ReferenceEdge>* refEdges, vector<Label>* nodeLabels, bool withNodeWeights);

		void clearMembers( void );

		int _nCount;
		int _eCount;
		PackedNode* _packedNodes;
		PackedEdge* _packedEdges;
		vector<Label>* _indexedLabels;

		NodeStructureInfoContainer* _nodeStructureInfo;
		EdgeStructureInfoContainer* _edgeStructureInfo;

};


#endif