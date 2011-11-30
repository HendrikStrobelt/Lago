#ifndef DATA_READER_H
#define DATA_READER_H

#include <string>
#include <vector>
#include <iostream>

#include "../Node.hpp"
#include "../Edge.hpp"

using namespace std;

#define NO_WEIGHTS -1
#define NO_LABELS -1

class DataReader {

	public:
		DataReader( void );
		~DataReader( void );

		void setNodeFile(string filePath, int xCol=0, int yCol=1, int weightCol=2, int labelCol=3);
		void setEdgeFile(string filePath, int x1Col=0, int y1Col=1, int x2Col=2, int y2Col=3, int weightCol=4);

		bool readNextNode(vector<Node>* nodes);
		bool readNextEdge(vector<Edge>* edges);

	private:

		enum NODE_COLS {X, Y, N_WEIGHT, N_LABEL};
		enum EDGE_COLS {X1, Y1, X2, Y2, E_WEIGHT};

		char _line[1024];
		FILE* _nodeFile;
		FILE* _edgeFile;

		int _nodeCols[4];
		int _edgeCols[5];
};

#endif