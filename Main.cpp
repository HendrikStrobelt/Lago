#include <vld.h>

#include "dataAccess\QuadTree.hpp"
#include "Node.hpp"
#include "dataAccess\DataReader.hpp"

#include <iostream>
#include <vector>

using namespace std;

int main( int argc, const char* argv[] ) {

	DataReader dr;

	dr.setNodeFile("_Data//lineNodes.txt");

	vector<Node> nodes;

	bool read = true;
	while (read) {
		read = dr.readNextNode(&nodes);
	}


	QuadTree qt(&nodes);
	NodeStructureInfoContainer* nodeStruct = qt.getNodeStructureInfoContainer();

	ofstream dest;
	dest.open("D://tree.svg", ios::out);
	qt.printTree(&dest);
	dest.close();

	//write test
	ofstream nstruc("D://structure.xxx", ios::binary);
	nodeStruct->serialize(&nstruc);
	nstruc.close();
	delete nodeStruct;

	ofstream tree("D://tree.xxx", ios::binary);
	int size;
	PackedNode* packedNodes = qt.getPackedTree(&size);
	tree.write((const char*)packedNodes, size * sizeof(PackedNode));
	tree.close();

	for (int i = 0; i < size; i++) {
		cout << packedNodes[i].x << " " << packedNodes[i].y << " " << packedNodes[i].depth << " " << packedNodes[i].weight << "\n";
	}
	cout << "________________________" << "\n";

	delete packedNodes;

	//load them back
	ifstream nstruc2("D://structure.xxx", ios::binary);
	NodeStructureInfoContainer loadedStruct(&nstruc2);
	nstruc2.close();

	ifstream tree2("D://tree.xxx", ios::binary);
	PackedNode* loadedNodes = new PackedNode[size];
	tree2.read((char*)loadedNodes, size * sizeof(PackedNode));
	nstruc2.close();

	for (int i = 0; i < size; i++) {
		cout << loadedNodes[i].x << " " << loadedNodes[i].y << " " << loadedNodes[i].depth << " " << loadedNodes[i].weight << "\n";
	}


	delete loadedNodes;

}