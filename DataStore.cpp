#include "DataStore.hpp"
#include "helper\Dumping.hpp"
#include "dataAccess\DataReader.hpp"
#include "dataAccess\QuadTree.hpp"
#include "dataAccess\FlatNodeContainer.hpp"
#include "dataAccess\EdgeHierarchy.hpp"
#include "dataAccess\FlatEdgeContainer.hpp"

#include <iostream>
#include <fstream>
#include <boost\filesystem.hpp>

using namespace boost::filesystem;


DataStore::DataStore( void ) {
	_eCount = 0;
	_nCount = 0;
	_nodeStructureInfo = NULL;
	_edgeStructureInfo = NULL;
	_packedNodes = NULL;
	_packedEdges = NULL;
	_indexedLabels = NULL;
}

DataStore::~DataStore( void ) {
	clearMembers();
}


//public


const vector<Label>* DataStore::getIndexedLabels( void ) {
	return _indexedLabels;
}

const PackedNode* DataStore::getPackedNodes(int* size) {
	if (_nCount > 0) {
		*size = _nCount;
		return _packedNodes;
	} else {
		*size = 0;
		return NULL;
	}
}
		
const PackedEdge* DataStore::getPackedEdges(int* size) {
	if (_eCount > 0) {
		*size = _eCount;
		return _packedEdges;
	} else {
		*size = 0;
		return NULL;
	}
}

const NodeStructureInfoContainer* DataStore::getNodeStructureInfo( void ) {
	if (_nCount > 0) {
		return _nodeStructureInfo;
	} else {
		return NULL;
	}
}
	
const EdgeStructureInfoContainer* DataStore::getEdgeStructureInfo( void ) {
	if (_eCount > 0) {
		return _edgeStructureInfo;
	} else {
		return NULL;
	}
}

const bool DataStore::hasLabels( void ) {
	return (_indexedLabels != NULL);
}

const bool DataStore::hasEdges( void ) {
	return (_eCount > 0);
}

void DataStore::setData(vector<Node>* nodes, vector<ReferenceEdge>* refEdges, vector<Label>* nodeLabels, bool withNodeWeights) {
	processData(nodes,refEdges, nodeLabels, withNodeWeights);
}

//private


void DataStore::processData(vector<Node>* nodes, vector<ReferenceEdge>* refEdges, vector<Label>* nodeLabels, bool withNodeWeights) {
	
	cout << " (total of " << nodes->size() << " nodes)" << "\n";
//	cout << "     creating QuadTree";
	FlatNodeContainer fc(nodes);

	_nodeStructureInfo = fc.getNodeStructureInfoContainer(); //fast
//	cout << " (tree height " << _nodeStructureInfo->getMaxDepth() << ")" << "\n";

	if (refEdges != NULL) {
	
		//switch to internal format
		vector<Edge> edges;
		edges.reserve(refEdges->size());
		
		for (int i = 0; i < refEdges->size(); i++) {
			ReferenceEdge* refEdge = &refEdges->at(i);
			edges.push_back(Edge(&nodes->at(refEdge->n1Ref), &nodes->at(refEdge->n2Ref), refEdge));
		}

		cout << " (total of " << edges.size() << " edges)" << "\n";
//		cout << "     creating EdgeHierarchy";
		FlatEdgeContainer ec(&edges);

		_edgeStructureInfo = ec.getEdgeStructureInfoContainer(); //fast
		//cout << " (hierarchy height " << _edgeStructureInfo->getMaxDepth() << ")" << "\n";
		cout << "     extracting needed edge informations" << "\n";
		_packedEdges = ec.getPackedContainer(&_eCount);
	}

	cout << "     extracting needed node informations" << "\n";
	_packedNodes = fc.getPackedContainer(&_nCount);

	
	if (nodeLabels != NULL) {
		cout << "preparing labels" << "\n";

		//create own label weights ---  1. node weight | 2. degree  | 3. weight = 1
		if (withNodeWeights) {
			for (int i = 0; i < nodeLabels->size(); i++) {
				nodeLabels->at(i).weight = nodes->at(i).weight;
			}
		} else 
		if (refEdges != NULL) {
			for (int i = 0; i < nodeLabels->size(); i++) {
				nodeLabels->at(i).weight = 0.0f;
			}
			for (int i = 0; i < refEdges->size(); i++) {
				nodeLabels->at(refEdges->at(i).n1Ref).weight++;
				nodeLabels->at(refEdges->at(i).n2Ref).weight++;
			}
		} else {
			for (int i = 0; i < nodeLabels->size(); i++) {
				nodeLabels->at(i).weight = 1.0f;
			}
		}
		
		_indexedLabels = nodeLabels;
	}
}



void DataStore::clearMembers( void ) {
	delete _nodeStructureInfo;
	delete _edgeStructureInfo;
	delete _packedNodes;
	delete _packedEdges;
	delete _indexedLabels;

	_eCount = 0;
	_nCount = 0;
	_nodeStructureInfo = NULL;
	_edgeStructureInfo = NULL;
	_packedNodes = NULL;
	_packedEdges = NULL;
	_indexedLabels = NULL;
}