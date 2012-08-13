#ifndef DATA_READER_H
#define DATA_READER_H

#include <string>
#include <vector>
#include <iostream>

#include "../Node.hpp"
#include "../Edge.hpp"
#include "../Label.hpp"

using namespace std;

#define NO_WEIGHTS -1
#define NO_LABELS -1
#define NO_LABEL_WEIGHTS -1

class DataReader {

	public:
		DataReader( void );
		~DataReader( void );

		void setNodeFile(string filePath);
		void setEdgeFile(string filePath);

		bool hasNodeLabels( void );
		bool hasNodeLabelWeights( void );

		bool readNextNode(vector<Node>* nodes, vector<Label>* labels=NULL);
		bool readNextEdge(vector<ReferenceEdge>* edges);

	private:

		enum NODE_COLS {X, Y, N_WEIGHT, N_LABEL, N_LABEL_WEIGHT};
		enum EDGE_COLS {N1, N2, E_WEIGHT};

		char _line[1024];
		bool _nodeFileSet;
		bool _edgeFileSet;
		FILE* _nodeFile;
		FILE* _edgeFile;

		int _nodeCols[5];
		int _edgeCols[3];
};

#endif