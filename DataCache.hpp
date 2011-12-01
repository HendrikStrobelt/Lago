#ifndef DATA_CACHE_H
#define DATA_CACHE_H

#include <string>
#include "Node.hpp"
#include "Edge.hpp"
#include "dataAccess\NodeStructureInfoContainer.hpp"
#include "dataAccess\EdgeStructureInfoContainer.hpp"

using namespace std;

class DataCache {
	
	public:
		DataCache( void );
		~DataCache( void );

		void loadDataSet(string nodeFile, string edgeFile = "");

		const PackedNode* getPackedNodes(int* size);
		const PackedEdge* getPackedEdges(int* size);
		const NodeStructureInfoContainer* getNodeStrucutreInfo( void );
		const EdgeStructureInfoContainer* getEdgeStrucutreInfo( void );

	private:
		string getDumpName(string nodeFile, string edgeFile);
		void loadFromFiles(string nodeFile, string edgeFile);
		void loadFromDump(string dumpName, bool loadEdges);
		void writeToDump(string dumpName, bool writeEdges);

		void clearMembers( void );

		int _nCount;
		int _eCount;
		PackedNode* _packedNodes;
		PackedEdge* _packedEdges;
		NodeStructureInfoContainer* _nodeStructureInfo;
		EdgeStructureInfoContainer* _edgeStructureInfo;

};


#endif