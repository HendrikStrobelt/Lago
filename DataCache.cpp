#include "DataCache.hpp"
#include "helper\Dumping.hpp"
#include "dataAccess\DataReader.hpp"
#include "dataAccess\QuadTree.hpp"
#include "dataAccess\EdgeHierarchy.hpp"

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

const NodeStructureInfoContainer* DataCache::getNodeStrucutreInfo( void ) {
	if (_nCount > 0) {
		return _nodeStructureInfo;
	} else {
		return NULL;
	}
}
	
const EdgeStructureInfoContainer* DataCache::getEdgeStrucutreInfo( void ) {
	if (_eCount > 0) {
		return _edgeStructureInfo;
	} else {
		return NULL;
	}
}

//private


string DataCache::getDumpName(string nodeFile, string edgeFile) {
	string dumpName = "_DataDump//";
	
	path nodePath(nodeFile);
	dumpName.append(nodePath.filename().stem().string());

	if (!edgeFile.empty()) {
		path edgePath(edgeFile);
		dumpName.append(edgePath.filename().stem().string());
	}
	dumpName.append(".hgv");

	return dumpName;
}
	

void DataCache::loadFromFiles(string nodeFile, string edgeFile) {
	DataReader dr;
	
	//load node data
	dr.setNodeFile(nodeFile);
	vector<Node> nodes;

	cout << "loading nodes from " << nodeFile;

	bool read = true;
	while (read) {
		read = dr.readNextNode(&nodes);
	}

	cout << " (total of " << nodes.size() << " nodes)" << "\n";
	cout << "     creating QuadTree";
	QuadTree qt(&nodes);

	_nodeStructureInfo = qt.getNodeStructureInfoContainer(); //fast
	cout << " (tree height " << _nodeStructureInfo->getMaxDepth() << ")" << "\n";
	cout << "     extracting needed informations" << "\n";
	_packedNodes = qt.getPackedTree(&_nCount);

	bool writeEdges = false;
	if (!edgeFile.empty()) {
		//load edge data
		writeEdges = true;
		dr.setEdgeFile(edgeFile);
		vector<Edge> edges;

		cout << "loading edges from " << edgeFile;

		read = true;
		while (read) {
			read = dr.readNextEdge(&edges);
		}

		cout << " (total of " << edges.size() << " edges)" << "\n";
		cout << "     creating EdgeHierarchy";
		EdgeHierarchy eh(&edges, &qt);

		_edgeStructureInfo = eh.getEdgeStructureInfoContainer(); //fast
		cout << " (tree height " << _edgeStructureInfo->getMaxDepth() << ")" << "\n";
		cout << "     extracting needed informations" << "\n";
		_packedEdges = eh.getPackedHierarchy(&_eCount);
	}

	cout << "saving extracted node/edge data to _DataDump for faster access" << "\n";
	writeToDump(getDumpName(nodeFile, edgeFile), writeEdges);
}


void DataCache::writeToDump(string dumpName, bool writeEdges) {
	
	path dir(dumpName);
	if (!exists(dir)) {
        create_directory(dir); 
	}

	//load nodes
		string nodeFile = dumpName;
		nodeFile.append("//packedNodes.bin");
		ofstream packedNodes(nodeFile, ios::binary);
		dump::w(&packedNodes, _nCount, _packedNodes);
		packedNodes.close();
	//load node structure info
		string nodeInfoFile = dumpName;
		nodeInfoFile.append("//nodeStructureInfo.bin");
		ofstream nodeInfos(nodeInfoFile, ios::binary);
		_nodeStructureInfo->serialize(&nodeInfos);
		nodeInfos.close();

	if (writeEdges) {
		//load edges
			string edgeFile = dumpName;
			edgeFile.append("//packedEdges.bin");
			ofstream packedEdges(edgeFile, ios::binary);
			dump::w(&packedEdges, _eCount, _packedEdges);
			packedEdges.close();
		//load node structure info
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

	cout << "loaded " << _nodeStructureInfo->getAllPoints(_nodeStructureInfo->getMaxDepth()) << " nodes";

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

			cout << " and " << _edgeStructureInfo->getAllPoints(_edgeStructureInfo->getMaxDepth()) << " edges";
	}

	cout << " from _DataDump" << "\n";
}


void DataCache::clearMembers( void ) {
	if (_nodeStructureInfo != NULL) {
		delete _nodeStructureInfo;
	}
	if (_edgeStructureInfo != NULL) {
		delete _edgeStructureInfo;
	}
	if (_packedNodes != NULL) {
		delete _packedNodes;
	}
	if (_packedEdges != NULL) {
		delete _packedEdges;
	}

	_eCount = 0;
	_nCount = 0;
	_nodeStructureInfo = NULL;
	_edgeStructureInfo = NULL;
	_packedNodes = NULL;
	_packedEdges = NULL;
}