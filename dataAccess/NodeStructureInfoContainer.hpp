#ifndef NODE_STRUCTURE_INFO_CONTAINER_H
#define NODE_STRUCTURE_INFO_CONTAINER_H

#include <vector>
#include <fstream>

using namespace std;

class NodeStructureInfoContainer {

	public:
		NodeStructureInfoContainer(int maxDepth, float width, float height, float left, float bottom, vector<int> leafNodes, vector<int> allNodes);
		~NodeStructureInfoContainer( void );
		
		
		int getMaxDepth( void );
		float getWidth( void );
		float getHeight( void );
		float getLeftBorder( void );
		float getBottomBorder( void );

		int getJoinDepth(float desiredMinDistance);
		int getRenderPoints(int depth);
		int getAllPoints(int depth);

		//dump to disc
		NodeStructureInfoContainer(ifstream* dataStream);
		void serialize(ofstream* dataStream);

	private:
		int _maxDepth;
		float _width;
		float _height;
		float _left;
		float _bottom;

		vector<int> _leafNodes;
		vector<int> _allNodes;
};


#endif