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
	
	d.loadDataSet("_Data//Hierarchy2Node.out", "_Data//Hierarchy2Edge.out");


	cout << "done";
}