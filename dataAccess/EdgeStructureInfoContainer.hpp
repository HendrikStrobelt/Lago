#ifndef EDGE_STRUCTURE_INFO_CONTAINER_H
#define EDGE_STRUCTURE_INFO_CONTAINER_H

#include <vector>
#include <fstream>

using namespace std;

class EdgeStructureInfoContainer {

	public:
		EdgeStructureInfoContainer(int maxDepth, vector<int> leafNodes, vector<int> allNodes);
		~EdgeStructureInfoContainer( void );
		
		int getMaxDepth( void );
		int getRenderEdges(int depth);
		int getAllEdges(int depth);

		//dump to disc
		EdgeStructureInfoContainer(ifstream* dataStream);
		void serialize(ofstream* dataStream);

	private:
		int _maxDepth;
		vector<int> _leafNodes;
		vector<int> _allNodes;
};


#endif