#include "QuadTree.hpp"
#include <numeric>
#include <algorithm>

QuadTree::QuadTree(const vector<Node>* nodes) {

	//define borders
	float minX = numeric_limits<float>::max();
	float maxX = numeric_limits<float>::min();
	float minY = numeric_limits<float>::max();
	float maxY = numeric_limits<float>::min();

	for (int i = 0; i < nodes->size(); i++) {
		minX = min(nodes->at(i).x, minX);
		maxX = max(nodes->at(i).x, maxX);
		minY = min(nodes->at(i).y, minY);
		maxY = max(nodes->at(i).y, maxY);
	}

	_width = maxX - minX;
	_height = maxY - minY;
	_left = minX;
	_bottom = minY;


	//insert the first point
	_metaVectors.push_back(new vector<QuadTreeMetaNode>);
	_nodeVectors.push_back(new vector<Node>);
	_metaVectors[0]->push_back(QuadTreeMetaNode());
	_nodeVectors[0]->push_back(nodes->at(0));
	_leafCount.push_back(0);


	//insert further points
	for (int i = 1; i < nodes->size(); i++) {
		//point x,y   point w ...
		insert(nodes->at(i), TravelIndex(0, 0, _left, _bottom, (_width / 2.0f), (_height / 2.0f)));
	}

	//initialize inner points
	_maxDepth = _metaVectors.size() - 1;
	initializeTree(0, 0);
	finalizeNode(getMeta(0,0), getNode(0, 0), isLeaf(getMeta(0, 0)), 0); //finalize head
}

QuadTree::~QuadTree( void ) {
	for (int i = 0; i < _metaVectors.size(); i++) {
		delete _metaVectors[i];
	}
	for (int i = 0; i < _nodeVectors.size(); i++) {
		delete _nodeVectors[i];
	}
}




//public methods


NodeStructureInfoContainer* QuadTree::getNodeStructureInfoContainer( void ) {
	vector<int> allNodes;
	for (int i = 0; i < _nodeVectors.size(); i++) {
		allNodes.push_back(_nodeVectors[i]->size());
	}
	return new NodeStructureInfoContainer(_maxDepth, _width, _height, _left, _bottom, _leafCount, allNodes);
}

PackedNode* QuadTree::getPackedTree(int* size) {
	int arraySize = 0;
	for (int i = 0; i < _nodeVectors.size(); i++) {
		arraySize += _nodeVectors[i]->size();
	}
	PackedNode* pn = new PackedNode[arraySize];
	
	int off = 0;
	for (int i = 0; i < _nodeVectors.size(); i++) {
		random_shuffle(_nodeVectors[i]->begin(), _nodeVectors[i]->end());
		for (int j = 0; j < _nodeVectors[i]->size(); j++) {
			pn[off + j] = _nodeVectors[i]->at(j);
		}

		off += _nodeVectors[i]->size();
	}

	*size = arraySize;
	return pn;
}


void QuadTree::printTree(ofstream* dest) const {
	TravelIndex tIndex(0, 0, _left, _bottom, (_width / 2.0f), (_height / 2.0f));

	(*dest) << "<?xml version=\"1.0\" standalone=\"no\"?>"
			<< "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" "
			<< "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">"
			<< "<svg width=\""
			<< 1600 << "px"
			<< "\" height=\""
			<< 900 << "px"
			<< "\" viewBox=\""
			<< _left
			<< " "
			<< (-_height)
			<< " "
			<< _width
			<< " "
			<< _height
			<< "\" "
			<< "xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">";


	printChilds(dest, tIndex);

	(*dest) << "</svg>";
}



//private methods

void QuadTree::printChilds(ofstream* dest, TravelIndex tIndex) const {
	if (tIndex.index != -1) {
		//node exists

		//print it
		(*dest) << "<rect x=\""
				<< tIndex.left
				<< "\" y=\""
				<< -((tIndex.bottom + (tIndex.height_half * 2.0f)) - _bottom)
				<< "\" width=\""
				<< (tIndex.width_half * 2.0f)
				<< "\" height=\""
				<< (tIndex.height_half * 2.0f)
				<< "\" fill=\"none\" stroke=\"black\" stroke-width=\"1\"/>";

		//recursive calls use travel down to construct correct borders

		QuadTreeMetaNode* m = getMeta(tIndex.level, tIndex.index);

		if (m->child1 != -1) {
			TravelIndex travChild1(tIndex);
			travelDown(1, &travChild1);
			printChilds(dest, travChild1);
		}
		if (m->child2 != -1) {
			TravelIndex travChild2(tIndex);
			travelDown(2, &travChild2);
			printChilds(dest, travChild2);
		}
		if (m->child3 != -1) {
			TravelIndex travChild3(tIndex);
			travelDown(3, &travChild3);
			printChilds(dest, travChild3);
		}
		if (m->child4 != -1) {
			TravelIndex travChild4(tIndex);
			travelDown(4, &travChild4);
			printChilds(dest, travChild4);
		}
	}
}


//sets weights and calculates inner node coordinates
void QuadTree::initializeTree(int level, int index) {

	QuadTreeMetaNode* m = getMeta(level, index);
	Node* n = getNode(level, index);

	if (isLeaf(m)) {
		//leaf recursion ends
		n->x *= m->n_Count; //if a leaf has a weight bigger than 1 (multiple points with same coords)
		n->y *= m->n_Count; //we have to multiply the coordintates for correct accumulation
		_leafCount[level]++;
	} else {
		//inner Node init

		int cLevel = level + 1;

		if (m->child1 != -1) {
			getMeta(cLevel, m->child1)->parent = index;
			initializeTree(cLevel, m->child1);
			collectChild(n, getNode(cLevel, m->child1));
			collectChild(m, getMeta(cLevel, m->child1));
			finalizeNode(getMeta(cLevel, m->child1), getNode(cLevel, m->child1), isLeaf(getMeta(cLevel, m->child1)), cLevel);		
		}
		if (m->child2 != -1) {
			getMeta(cLevel, m->child2)->parent = index;
			initializeTree(cLevel, m->child2);
			collectChild(n, getNode(cLevel, m->child2));
			collectChild(m, getMeta(cLevel, m->child2));
			finalizeNode(getMeta(cLevel, m->child2), getNode(cLevel, m->child2), isLeaf(getMeta(cLevel, m->child2)), cLevel);		
		}
		if (m->child3 != -1) {
			getMeta(cLevel, m->child3)->parent = index;
			initializeTree(cLevel, m->child3);
			collectChild(n, getNode(cLevel, m->child3));
			collectChild(m, getMeta(cLevel, m->child3));
			finalizeNode(getMeta(cLevel, m->child3), getNode(cLevel, m->child3), isLeaf(getMeta(cLevel, m->child3)), cLevel);		
		}
		if (m->child4 != -1) {
			getMeta(cLevel, m->child4)->parent = index;
			initializeTree(cLevel, m->child4);
			collectChild(n, getNode(cLevel, m->child4));
			collectChild(m, getMeta(cLevel, m->child4));
			finalizeNode(getMeta(cLevel, m->child4), getNode(cLevel, m->child4), isLeaf(getMeta(cLevel, m->child4)), cLevel);		
		}
	}
}


//collect point count from the child and add it to the node
void QuadTree::collectChild(QuadTreeMetaNode* m, QuadTreeMetaNode* child) {
	m->n_Count += child->n_Count;
}


//collect point and weight data from the child and add it to the node
void QuadTree::collectChild(Node* n, Node* child) {
	n->weight += child->weight;
	n->x += child->x;
	n->y += child->y;
}

//divide accumulated x and y values through the weight of the node
void QuadTree::finalizeNode(QuadTreeMetaNode* m, Node* n, bool isLeaf, int depth) {
	n->x /= m->n_Count;
	n->y /= m->n_Count;
	n->depth = depth;
	if (!isLeaf) {
		n->depth = 0.0f - n->depth; //inner nodes can be identified by negative depths
	}
}

void QuadTree::insert(const Node n, TravelIndex tIndex) {

	float parent_bottom;
	float parent_left;
	int childMarker;
	int parentIndex;

	//search the correct place
	while (tIndex.index != -1) {
		parentIndex = tIndex.index; //store everything go one step deeper
		parent_bottom = tIndex.bottom;
		parent_left = tIndex.left;

		childMarker = travelDown(travelSelect(n.x, n.y, &tIndex), &tIndex);
	}


	QuadTreeMetaNode* parent = getMeta(tIndex.level-1, parentIndex);
	Node* parentNode = getNode(tIndex.level-1, parentIndex);

	//parent is a leaf node and has the same coordinates as the point that should be inserted
	//epsilon?
	if (isLeaf(parent) && (parentNode->x == n.x) && (parentNode->y == n.y)) {
		//do not insert the point instead increase the weight and node count
		parentNode->weight += n.weight;
		parent->n_Count++;
	} else {

		//check if the index level exits
		if (_metaVectors.size() <= tIndex.level) { //add new level
			_metaVectors.push_back(new vector<QuadTreeMetaNode>);
			_nodeVectors.push_back(new vector<Node>);

			_leafCount.push_back(0);
		}

		//insert the new node
		_metaVectors[tIndex.level]->push_back(QuadTreeMetaNode());
		_nodeVectors[tIndex.level]->push_back(n);
		
		bool leaf = isLeaf(parent);
		switch(childMarker) { //add index pointer to parent
			case 1: parent->child1 = (_metaVectors[tIndex.level]->size() - 1); break;
			case 2: parent->child2 = (_metaVectors[tIndex.level]->size() - 1); break;
			case 3: parent->child3 = (_metaVectors[tIndex.level]->size() - 1); break;
			case 4: parent->child4 = (_metaVectors[tIndex.level]->size() - 1); break;
		}

		if (leaf) {
			//parent is a leaf node and has to be transformed into a inner node
			
			Node parent_node(*parentNode); //store point data

			parentNode->x = 0.0f; //transform to inner node
			parentNode->y = 0.0f;		
			parentNode->weight = 0;

			parent->n_Count = 0;

			//reinsert the point
			insert(parent_node, TravelIndex((tIndex.level -1), parentIndex, parent_left, parent_bottom, (tIndex.width_half * 2.0f) , (tIndex.height_half * 2.0f)));
		}
	}
}



inline int QuadTree::travelSelect(float x, float y, const TravelIndex* tIndex) const {
	int childMarker;

	if (x > (tIndex->left + tIndex->width_half)) {
	//2 || 4
		if (y > (tIndex->bottom + tIndex->height_half)) {
			childMarker = 2;
		} else {
			childMarker = 4;
		}
	} else {
	// 1 || 3
		if (y > (tIndex->bottom + tIndex->height_half)) {
			childMarker = 1;
		} else {
			childMarker = 3;
		}
	}

	return childMarker;
}

//changes the submitted travel index 
//child marker is returned as is to ease the use together with travelSelect
inline int QuadTree::travelDown(int childMarker, TravelIndex* tIndex) const {

	if (childMarker == 2) {
		tIndex->index = getMeta(tIndex->level, tIndex->index)->child2;
		tIndex->bottom = tIndex->bottom + tIndex->height_half;
		tIndex->left = tIndex->left + tIndex->width_half;
	} else
	if (childMarker == 4) {
		tIndex->index = getMeta(tIndex->level, tIndex->index)->child4;
		tIndex->left = tIndex->left + tIndex->width_half;
	} else 
	if (childMarker == 1) {
		tIndex->index = getMeta(tIndex->level, tIndex->index)->child1;
		tIndex->bottom = tIndex->bottom + tIndex->height_half;
	} else
	if (childMarker == 3) {
		tIndex->index = getMeta(tIndex->level, tIndex->index)->child3;
	}

	tIndex->width_half /= 2.0f;
	tIndex->height_half /= 2.0f;
	tIndex->level++;

	return childMarker;
}

inline Node* QuadTree::getNode(int level, int index) const{
	return &_nodeVectors[level]->at(index);
}

inline QuadTreeMetaNode* QuadTree::getMeta(int level, int index) const {
	return &_metaVectors[level]->at(index);
}


inline bool QuadTree::isLeaf(QuadTreeMetaNode* n) const {
	if (n->child1 == -1 && n->child2 == -1 && n->child3 == -1 && n->child4 == -1) {
		return true;
	} else {
		return false;
	}
}