#include "DataCache.hpp"
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


DataCache::DataCache( void ) {
	_eCount = 0;
	_nCount = 0;
	_nodeStructureInfo = NULL;
	_edgeStructureInfo = NULL;
	_packedNodes = NULL;
	_packedEdges = NULL;
	_indexedLabels = NULL;
}

DataCache::~DataCache( void ) {
	clearMembers();
}


//public

void DataCache::loadDataSet(string nodeFile, string edgeFile) {
	clearMembers();
	string dumpName = getDumpName(nodeFile, edgeFile);
	path dumpPath(dumpName);
	
	if (exists(dumpPath) && is_directory(dumpPath)) {
		//dump exists
		bool loadEdges = true;
		if (edgeFile.empty()) {
			loadEdges = false;
		}
		loadFromDump(dumpName, loadEdges);
	} else {
		//no dump exists 
		loadFromFiles(nodeFile, edgeFile);
	}
}

const vector<Label>* DataCache::getIndexedLabels( void ) {
	return _indexedLabels;
}

const PackedNode* DataCache::getPackedNodes(int* size) {
	if (_nCount > 0) {
		*size = _nCount;
		return _packedNodes;
	} else {
		*size = 0;
		return NULL;
	}
}
		
const PackedEdge* DataCache::getPackedEdges(int* size) {
	if (_eCount > 0) {
		*size = _eCount;
		return _packedEdges;
	} else {
		*size = 0;
		return NULL;
	}
}

const NodeStructureInfoContainer* DataCache::getNodeStructureInfo( void ) {
	if (_nCount > 0) {
		return _nodeStructureInfo;
	} else {
		return NULL;
	}
}
	
const EdgeStructureInfoContainer* DataCache::getEdgeStructureInfo( void ) {
	if (_eCount > 0) {
		return _edgeStructureInfo;
	} else {
		return NULL;
	}
}

const bool DataCache::hasLabels( void ) {
	return (_indexedLabels != NULL);
}


//private


string DataCache::getDumpName(string nodeFile, string edgeFile) {
	string dumpName = "_DataDump//";
	
	path nodePath(nodeFile);
	dumpName.append(nodePath.filename().stem().string());

	if (!edgeFile.empty()) {
		path edgePath(edgeFile);
		dumpName.append(edgePath.filename().stem().string());
		if (!EDGE_HIERARCHY_FLAT) {
			dumpName.append("_H");
		}
	}
	dumpName.append(".hgv");

	return dumpName;
}
	

void DataCache::loadFromFiles(string nodeFile, string edgeFile) {
	DataReader dr;
	
	//load node data
	dr.setNodeFile(nodeFile);
	vector<Node> nodes;
	vector<Label>* labels = new vector<Label>();

	cout << "loading nodes from " << nodeFile;

	bool read = true;
	while (read) {
		read = dr.readNextNode(&nodes, labels);
	}

	cout << " (total of " << nodes.size() << " nodes)" << "\n";
//	cout << "     creating QuadTree";
	FlatNodeContainer fc(&nodes);

	_nodeStructureInfo = fc.getNodeStructureInfoContainer(); //fast
//	cout << " (tree height " << _nodeStructureInfo->getMaxDepth() << ")" << "\n";

	bool writeEdges = false;
	if (!edgeFile.empty()) {
		//load edge data
		writeEdges = true;
		dr.setEdgeFile(edgeFile);
		vector<ReferenceEdge> refEdges;

		cout << "loading edges from " << edgeFile;

		read = true;
		while (read) {
			read = dr.readNextEdge(&refEdges);
		}
		//switch to internal format
		vector<Edge> edges;
		edges.reserve(refEdges.size());
		for (int i = 0; i < refEdges.size(); i++) {
			ReferenceEdge* refEdge = &refEdges[i];
			edges.push_back(Edge(&nodes[refEdge->n1Ref], &nodes[refEdge->n2Ref], refEdge));
		}

		cout << " (total of " << edges.size() << " edges)" << "\n";
//		cout << "     creating EdgeHierarchy";
		FlatEdgeContainer ec(&edges);

		_edgeStructureInfo = ec.getEdgeStructureInfoContainer(); //fast
		//cout << " (hierarchy height " << _edgeStructureInfo->getMaxDepth() << ")" << "\n";
		cout << "     extracting needed edge informations" << "\n";
		_packedEdges = ec.getPackedContainer(&_eCount);


		if (dr.hasNodeLabels() && !dr.hasNodeLabelWeights()) {
			//create own label weights --- degree
			for (int i = 0; i < labels->size(); i++) {
				labels->at(i).weight = 0.0f;
			}
			for (int i = 0; i < refEdges.size(); i++) {
				labels->at(refEdges[i].n1Ref).weight++;
				labels->at(refEdges[i].n2Ref).weight++;
			}
		}
	}

	cout << "     extracting needed node informations" << "\n";
	_packedNodes = fc.getPackedContainer(&_nCount);

	bool writeLabels = false;
	if (dr.hasNodeLabels()) {
		cout << "preparing labels" << "\n";
		writeLabels = true;
		_indexedLabels = labels;
	} else {
		delete labels;
	}

	cout << "saving extracted node/edge data to _DataDump for faster access" << "\n";
	writeToDump(getDumpName(nodeFile, edgeFile), writeEdges, writeLabels);
}


void DataCache::writeToDump(string dumpName, bool writeEdges, bool writeLabels) {
	
	path dir(dumpName);
	if (!exists(dir)) {
        create_directory(dir); 
	}

	//write nodes
		string nodeFile = dumpName;
		nodeFile.append("//packedNodes.bin");
		ofstream packedNodes(nodeFile, ios::binary);
		dump::w(&packedNodes, _nCount, _packedNodes);
		packedNodes.close();
	//write node structure info
		string nodeInfoFile = dumpName;
		nodeInfoFile.append("//nodeStructureInfo.bin");
		ofstream nodeInfos(nodeInfoFile, ios::binary);
		_nodeStructureInfo->serialize(&nodeInfos);
		nodeInfos.close();

	if (writeLabels) {
		//write label vector
		string labelFile = dumpName;
		labelFile.append("//indexedLabels.bin");
		ofstream indexedLabels(labelFile, ios::binary);
		dump::w(&indexedLabels, _indexedLabels);
		indexedLabels.close();			
	}

	if (writeEdges) {
		//write edges
			string edgeFile = dumpName;
			edgeFile.append("//packedEdges.bin");
			ofstream packedEdges(edgeFile, ios::binary);
			dump::w(&packedEdges, _eCount, _packedEdges);
			packedEdges.close();
		//write node structure info
			string edgeInfoFile = dumpName;
			edgeInfoFile.append("//edgeStructureInfo.bin");
			ofstream edgeInfos(edgeInfoFile, ios::binary);
			_edgeStructureInfo->serialize(&edgeInfos);
			edgeInfos.close();
	}

	cout << "done" << "\n";
}

void DataCache::loadFromDump(string dumpName, bool loadEdges) {
	//load nodes
		string nodeFile = dumpName;
		nodeFile.append("//packedNodes.bin");
		ifstream packedNodes(nodeFile, ios::binary);
		dump::r(&packedNodes, &_nCount, &_packedNodes);
		packedNodes.close();
	//load node structure info
		string nodeInfoFile = dumpName;
		nodeInfoFile.append("//nodeStructureInfo.bin");
		ifstream nodeInfos(nodeInfoFile, ios::binary);
		_nodeStructureInfo = new NodeStructureInfoContainer(&nodeInfos);
		nodeInfos.close();

	cout << "loaded " << _nodeStructureInfo->getRenderNodes(_nodeStructureInfo->getMaxDepth()) << "/";
	cout << _nodeStructureInfo->getAllNodes(_nodeStructureInfo->getMaxDepth()) << " nodes";

	//load edges
		string labelFile = dumpName;
		labelFile.append("//indexedLabels.bin");
		path labelPath(labelFile);
	if (exists(labelPath)) {
		_indexedLabels = new vector<Label>();
		ifstream indexedLabels(labelFile, ios::binary);
		dump::r(&indexedLabels, _indexedLabels);
		indexedLabels.close();
		cout << " (with labels)";
	}

	if (loadEdges) {
		//load edges
			string edgeFile = dumpName;
			edgeFile.append("//packedEdges.bin");
			ifstream packedEdges(edgeFile, ios::binary);
			dump::r(&packedEdges, &_eCount, &_packedEdges);
			packedEdges.close();
		//load node structure info
			string edgeInfoFile = dumpName;
			edgeInfoFile.append("//edgeStructureInfo.bin");
			ifstream edgeInfos(edgeInfoFile, ios::binary);
			_edgeStructureInfo = new EdgeStructureInfoContainer(&edgeInfos);
			edgeInfos.close();

			cout << " and " << _edgeStructureInfo->getRenderEdges(_edgeStructureInfo->getMaxDepth()) << "/";
			cout << _edgeStructureInfo->getAllEdges(_edgeStructureInfo->getMaxDepth()) << " edges";
	}

	cout << " from _DataDump" << "\n";
}


void DataCache::clearMembers( void ) {
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