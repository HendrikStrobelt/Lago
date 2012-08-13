#ifndef DATA_CACHE_H
#define DATA_CACHE_H

#include <string>
#include "Node.hpp"
#include "Edge.hpp"
#include "Label.hpp"
#include "dataAccess\NodeStructureInfoContainer.hpp"
#include "dataAccess\EdgeStructureInfoContainer.hpp"

using namespace std;

class DataCache {
	
	public:
		DataCache( void );
		~DataCache( void );

		void loadDataSet(string nodeFile, string edgeFile = "");

		const vector<Label>* getIndexedLabels( void );
		const PackedNode* getPackedNodes(int* size);
		const PackedEdge* getPackedEdges(int* size);
		const NodeStructureInfoContainer* getNodeStructureInfo( void );
		const EdgeStructureInfoContainer* getEdgeStructureInfo( void );

		const bool hasLabels( void );

	private:
		string getDumpName(string nodeFile, string edgeFile);
		void loadFromFiles(string nodeFile, string edgeFile);
		void loadFromDump(string dumpName, bool loadEdges);
		void writeToDump(string dumpName, bool writeEdges, bool writeLabels);

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