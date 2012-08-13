#ifndef NODE_STRUCTURE_INFO_CONTAINER_H
#define NODE_STRUCTURE_INFO_CONTAINER_H

#include <vector>
#include <fstream>

using namespace std;

class NodeStructureInfoContainer {

	public:
		NodeStructureInfoContainer(int maxDepth, float width, float height, float left, float bottom, vector<int> leafNodes, vector<int> allNodes);
		~NodeStructureInfoContainer( void );
		
		
		int getMaxDepth( void ) const;
		float getWidth( void ) const;
		float getHeight( void ) const;
		float getLeftBorder( void ) const;
		float getBottomBorder( void ) const;

		int getJoinDepth(float desiredMinDistance) const;
		int getRenderNodes(int depth) const;
		int getAllNodes(int depth) const;

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