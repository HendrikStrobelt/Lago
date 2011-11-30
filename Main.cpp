#include <vld.h>

#include "dataAccess\DataReader.hpp"


int main( int argc, const char* argv[] ) {


	DataReader dr;
	dr.setNodeFile("NodeTest.txt");

	for (int i = 0; i < 10; i++) {
		Node* n = dr.readNextNode();
		cout << n->x << " " << n->y << " " << n->weight << "\n";
		delete n;
	}


	dr.setNodeFile("NodeTest2.txt");
	dr.setEdgeFile("EdgeTest.txt");
	for (int i = 0; i < 10; i++) {
		Node* n = dr.readNextNode();
		if (n != NULL) {
			cout << n->x << " " << n->y << " " << n->weight << "\n";
			delete n;
		}

		Edge* e = dr.readNextEdge();
		cout << e->x1 << " " << e->y1 << " " << e->x2 << " " << e->y2 << " " << e->weight << "\n";
		delete e;
	}


}