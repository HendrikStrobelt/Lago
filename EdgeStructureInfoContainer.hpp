#ifndef EDGE_STRUCTURE_INFO_CONTAINER_H
#define EDGE_STRUCTURE_INFO_CONTAINER_H

#include <vector>
#include <fstream>

using namespace std;

class EdgeStructureInfoContainer {

	public:
		EdgeStructureInfoContainer( void );
		~EdgeStructureInfoContainer( void );
		
		int getMaxDepth( void );
		int getRenderPoints(int depth);
		int getAllPoints(int depth);

		//dump to disc
		EdgeStructureInfoContainer(ifstream* dataStream);
		void serialize(ofstream* dataStream);

	private:
		int _maxDepth;
		vector<int> _leafNodes;
		vector<int> _allNodes;
};


#endif