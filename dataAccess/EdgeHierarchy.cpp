#include "EdgeHierarchy.hpp"
#include "../GlobalConstants.hpp"
#include <algorithm>


EdgeHierarchy::EdgeHierarchy(const vector<Edge>* edges, const QuadTree* quadTree) {
	//store them for a short time to save variables in the recursive calls
	if (!EDGE_HIERARCHY_FLAT) {
		_maxDepth = -1;
		_quadTree = quadTree;
		_edges = edges;
			createHierarchy();
		_edges = NULL;
		_quadTree = NULL;
	} else {
		_maxDepth = 0;
		_edgeVectors.push_back(new vector<Edge>);
		for (int i = 0; i < edges->size(); i++) {
			Edge e(edges->at(i));
			e.depth = 0;
			_edgeVectors[0]->push_back(e);
		}
		_leafNodes.push_back(edges->size());
	}
}

EdgeHierarchy::~EdgeHierarchy( void ) {
	for (int i = 0; i < _edgeVectors.size(); i++) {
		delete _edgeVectors[i];
	}
}


//public

EdgeStructureInfoContainer* EdgeHierarchy::getEdgeStructureInfoContainer( void ) {
	vector<int> allNodes;
	for (int i = 0; i < _edgeVectors.size(); i++) {
		allNodes.push_back(_edgeVectors[i]->size());
	}
	return new EdgeStructureInfoContainer(_maxDepth, _leafNodes, allNodes);
}


PackedEdge* EdgeHierarchy::getPackedHierarchy(int* size) {
	int arraySize = 0;
	for (int i = 0; i < _edgeVectors.size(); i++) {
		arraySize += _edgeVectors[i]->size();
	}
	PackedEdge* pe = new PackedEdge[arraySize];
	
	int off = 0;
	for (int i = 0; i < _edgeVectors.size(); i++) {
		random_shuffle(_edgeVectors[i]->begin(), _edgeVectors[i]->end());
		for (int j = 0; j < _edgeVectors[i]->size(); j++) {
			pe[off + j] = _edgeVectors[i]->at(j);
		}

		off += _edgeVectors[i]->size();
	}

	*size = arraySize;
	return pe;
}

//private

void EdgeHierarchy::addEdge(Edge e, int level) {
	//if necessary add levels
	while (level > _maxDepth) {
		_edgeVectors.push_back(new vector<Edge>);
		_leafNodes.push_back(0);
		_maxDepth++;
	}
	if (e.depth > 0) { //leaf edge
		_leafNodes[level]++;
	}

	_edgeVectors[level]->push_back(e);
}

//private methods that create the hierarchy

void EdgeHierarchy::createHierarchy( void ) {
	QuadTreeMetaNode n0 = _quadTree->_metaVectors[0]->at(0);
	if (_edges->size() > 0) {
		TravelIndex tIndex(0, 0, _quadTree->_left, _quadTree->_bottom, (_quadTree->_width / 2.0f), (_quadTree->_height / 2.0f));
		vector<int> indices;
		float weightSum = 0;
		for (int i = 0; i < _edges->size(); i++) {
			indices.push_back(i);
			weightSum += _edges->at(i).weight;
		}
		divideEdges(tIndex, tIndex, &indices, weightSum);
	}
}


void EdgeHierarchy::divideEdges(TravelIndex t1, TravelIndex t2, vector<int>* indices, float weightSum) {

	QuadTreeMetaNode* m1 = _quadTree->getMeta(t1.level, t1.index);
	QuadTreeMetaNode* m2 = _quadTree->getMeta(t2.level, t2.index);

	if (_quadTree->isLeaf(m1) && _quadTree->isLeaf(m2)) {
		//both nodes are leafs add the edge as leaf edge and return
		short depth = t2.level; 
		if (t1.level > t2.level) {
			depth = t1.level;
		} 

		Edge tmp(_edges->at(indices->at(0)));
		tmp.depth = depth; //leaf edge positive depth
		addEdge(tmp, depth);
	} else {

		vector<int>* splitArray[4][4];
		float weightArray[4][4];
		for (int i = 0; i < 4; i++) {
			for (int j  = 0; j < 4; j++) {
				splitArray[i][j] = NULL;
				weightArray[i][j] = 0.0f;
			}
		}

		if (t1.level == t2.level && t1.index == t2.index) {
			//edge start and end point to the same meta node
			//can't be a leaf (-> if above)

			for (int index = 0; index < indices->size(); index++) {
				Edge e = _edges->at(indices->at(index));

				//test where the edge belongs to
				int i = _quadTree->travelSelect(e.x1, e.y1, &t1) - 1;
				int j = _quadTree->travelSelect(e.x2, e.y2, &t2) - 1; //t1=t2
				//insert it into the correct place
				if (splitArray[i][j] == NULL) {
					splitArray[i][j] = new vector<int>();
				}
				splitArray[i][j]->push_back(indices->at(index));
				weightArray[i][j] += _edges->at(indices->at(index)).weight;
			}			
		} else {
			//different nodes => insert edges
			Node* n1 = _quadTree->getNode(t1.level, t1.index);
			Node* n2 = _quadTree->getNode(t2.level, t2.index);
			int depth = t2.level; 
			if (t1.level > t2.level) {
				depth = t1.level;
			} 
			depth = 0 - depth; //inner edge indicated by negative depth

			addEdge(Edge(n1, n2, depth, weightSum), abs(depth));


			for (int index = 0; index < indices->size(); index++) {
				Edge e = _edges->at(indices->at(index));

				int i = 0;
				int j = 0;
				if (!_quadTree->isLeaf(m1)) {
					i = _quadTree->travelSelect(e.x1, e.y1, &t1) - 1;
				}
				if (!_quadTree->isLeaf(m2)) {
					j = _quadTree->travelSelect(e.x2, e.y2, &t2) - 1;
				}
				//insert it into the correct place
				if (splitArray[i][j] == NULL) {
					splitArray[i][j] = new vector<int>();
				}
				splitArray[i][j]->push_back(indices->at(index));
				weightArray[i][j] += _edges->at(indices->at(index)).weight;
			}			
		}


		//recursive calls

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (splitArray[i][j] != NULL) {
					TravelIndex newT1(t1);
					TravelIndex newT2(t2);

					//go on with t1
					if (!_quadTree->isLeaf(m1)) {
						_quadTree->travelDown((i+1), &newT1);
					}
					//and with t2
					if (!_quadTree->isLeaf(m2)) {
						_quadTree->travelDown((j+1), &newT2);
					}										

					if (splitArray[i][j]->size() > 1) {
						divideEdges(newT1, newT2, splitArray[i][j], weightArray[i][j]);
					} else {
						//speed up for single edges
						divideEdges(newT1, newT2, splitArray[i][j]->at(0));
					}
					delete splitArray[i][j];
				}
			}
		}
	}
}


//simplified version of divideEdges that accepts only one index => one edge
//it is a common case that one edge still goes deeper e.g. an edge between a high leaf in the tree
//and a node that goes deeper and deepper. Therefore a  lot of effort can be saved by providing
//a method that works with just one edge
void EdgeHierarchy::divideEdges(TravelIndex t1, TravelIndex t2, int index) {

	QuadTreeMetaNode* m1 = _quadTree->getMeta(t1.level, t1.index);
	QuadTreeMetaNode* m2 = _quadTree->getMeta(t2.level, t2.index);

	if (_quadTree->isLeaf(m1) && _quadTree->isLeaf(m2)) {
		//both nodes are leafs add the edge as leaf edge and return
		int depth = t2.level; 
		if (t1.level > t2.level) {
			depth = t1.level;
		} 

		Edge tmp(_edges->at(index));
		tmp.depth = depth; //leaf edge positive depth
		addEdge(tmp, depth);
	} else {

		int nodeIndexI;
		int nodeIndexJ;

		if (t1.level == t2.level && t1.index == t2.index) {
			//edge start and end point to the same meta node
			//can't be a leaf (-> if above)

			Edge e = _edges->at(index);

			//test where the edge belongs to
			nodeIndexI = _quadTree->travelSelect(e.x1, e.y1, &t1) - 1;
			nodeIndexJ = _quadTree->travelSelect(e.x2, e.y2, &t2) - 1; //t1=t2

		} else {
			//different nodes => insert edges
			Node* n1 = _quadTree->getNode(t1.level, t1.index);
			Node* n2 = _quadTree->getNode(t2.level, t2.index);
			int depth = t2.level; 
			if (t1.level > t2.level) {
				depth = t1.level;
			} 
			depth = 0 - depth; //inner edge indicated by negative depth
			float weight = _edges->at(index).weight;
			addEdge(Edge(n1, n2, depth, weight), abs(depth));

			Edge e = _edges->at(index);

			if (!_quadTree->isLeaf(m1)) {
				nodeIndexI = _quadTree->travelSelect(e.x1, e.y1, &t1) - 1;
			}
			if (!_quadTree->isLeaf(m2)) {
				nodeIndexJ = _quadTree->travelSelect(e.x2, e.y2, &t2) - 1;
			}
		}


		//recursive calls

		TravelIndex newT1(t1);
		TravelIndex newT2(t2);

		//go on with t1
		if (!_quadTree->isLeaf(m1)) {
			_quadTree->travelDown((nodeIndexI+1), &newT1);
		}
		//and with t2
		if (!_quadTree->isLeaf(m2)) {
			_quadTree->travelDown((nodeIndexJ+1), &newT2);
		}										

		divideEdges(newT1, newT2, index);
	}
}
