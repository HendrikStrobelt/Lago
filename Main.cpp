#include <vld.h>

#include "DataCache.hpp"
#include <iostream>
#include <vector>
#include "Node.hpp"
#include "Edge.hpp"
#include "dataAccess\NodeStructureInfoContainer.hpp"
#include "dataAccess\EdgeStructureInfoContainer.hpp"

using namespace std;

int main( int argc, const char* argv[] ) {

	DataCache d;
	
	d.loadDataSet("_Data//JournalNode.out", "_Data//JournalEdge.out");

/*	int size;
	const PackedNode* pn = d.getPackedNodes(&size);
	for (int i = 0; i < size; i++) {
		cout << pn[i].x << " " << pn[i].y << " " << pn[i].weight << "\n";
	}
*/



	cout << "done";
}