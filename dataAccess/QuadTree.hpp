#ifndef QUAD_TREE_H
#define QUAD_TREE_H

#include "../Node.hpp"
#include "NodeStructureInfoContainer.hpp"
#include <vector>
#include <fstream>

using namespace std;

/** node childs
 *
 *  1 | 2
 *  3 | 4
*/

struct QuadTreeMetaNode {
	int child1;
	int child2;
	int child3;
	int child4;
	int parent;
	int n_Count; //nr of nodes in the cluster (!= weight)
	QuadTreeMetaNode( void ) : child1(-1), child2(-1), child3(-1), child4(-1), parent(-1), n_Count(1) {};
};

struct TravelIndex { //everything one needs to travel down the three
	int level;
	int index;
	float left;
	float bottom;
	float width_half;
	float height_half;
	TravelIndex(int lev, int i, float pLeft, float pBottom, float w_half, float h_half) :
				level(lev), index(i), left(pLeft), bottom(pBottom), width_half(w_half), height_half(h_half) {};
};


class QuadTree {

	friend class EdgeHierarchyCreator;

    public:
        QuadTree(const vector<Node>* nodes);
		~QuadTree( void );
        
		NodeStructureInfoContainer* getNodeStructureInfoContainer( void );
		PackedNode* getPackedTree(int* size);

		//print method
		void printTree(ofstream* dest) const;
	private:

		//tree creation
		void insert(const Node n, TravelIndex tIndex);

		void initializeTree(int level, int index);
		void collectChild(Node* n, Node* child);
		void collectChild(QuadTreeMetaNode* m, QuadTreeMetaNode* child);
		void finalizeNode(QuadTreeMetaNode* m, Node* n, bool isLeaf, int depth);	
				
		//helpers
		inline int travelSelect(float x, float y, const TravelIndex* tIndex) const;
		inline int travelDown(int childMarker, TravelIndex* tIndex) const;
		
		inline bool isLeaf(QuadTreeMetaNode* m) const;
		inline QuadTreeMetaNode* getMeta(int level, int index) const;
		inline Node* getNode(int level, int index) const;

				
		//print helper method
		void printChilds(ofstream* dest, TravelIndex tIndex) const;
			
		float _bottom;
		float _left;
		float _width;
		float _height;
		int _maxDepth;

		vector<vector<QuadTreeMetaNode> *> _metaVectors;
		vector<vector<Node> *> _nodeVectors;
		vector<int> _leafCount;
};


#endif