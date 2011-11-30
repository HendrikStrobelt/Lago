#include "DataReader.hpp"

DataReader::DataReader( void ) {
	_nodeFile = NULL;
	_edgeFile = NULL;
}

DataReader::~DataReader( void ) {
	if (_nodeFile != NULL) {
		fclose(_nodeFile);
	}
	if (_edgeFile != NULL) {
		fclose(_edgeFile);
	}
}



void DataReader::setNodeFile(string filePath, int xCol, int yCol, int weightCol, int labelCol) {
	if (_nodeFile != NULL) {
		fclose(_nodeFile);
	}

	fopen_s(&_nodeFile, filePath.c_str(), "r");
	if (_nodeFile) {
		fgets(_line, sizeof(_line), _nodeFile); //read first line
		_nodeCols[X] = xCol;
		_nodeCols[Y] = yCol;
		_nodeCols[N_WEIGHT] = weightCol;
		_nodeCols[N_LABEL] = labelCol;
	} else {
		fclose(_nodeFile);
		_nodeFile = NULL;
		cerr << "ERROR: file open failed on " << filePath << "\n";
	}
}


void DataReader::setEdgeFile(string filePath, int x1Col, int y1Col, int x2Col, int y2Col, int weightCol) {
	if (_edgeFile != NULL) {
		fclose(_edgeFile);
	}

	fopen_s(&_edgeFile, filePath.c_str(), "r");
	if (_edgeFile) {
		fgets(_line, sizeof(_line), _edgeFile); //read first line
		_edgeCols[X1] = x1Col;
		_edgeCols[Y1] = y1Col;
		_edgeCols[X2] = x2Col;
		_edgeCols[Y2] = y2Col;
		_edgeCols[E_WEIGHT] = weightCol;
	} else {
		fclose(_edgeFile);
		_edgeFile = NULL;
		cerr << "ERROR: file open failed on " << filePath << "\n";
	}
}


Node* DataReader::readNextNode( void ) {
	if (_nodeFile != NULL && 
		fgets(_line, sizeof(_line), _nodeFile) != NULL) {

		Node* n = new Node();
		char* token = strtok(_line,",");

		int i = 0;
		while (token != NULL) {
			if (i == _nodeCols[X]) {
				n->x = (float)atof(token);
			} else
			if (i == _nodeCols[Y]) {
				n->y = (float)atof(token);
			} else
			if (i == _nodeCols[N_WEIGHT]) {
				n->weight = (float)atof(token);
			} else 
			if (i == _nodeCols[N_LABEL]) {
				//NOT YET SUPPORTED
			}						
		
			i++;
			token = strtok(NULL, ","); //next token
		}
		return n;
	} else {
		return NULL;
	}
}

Edge* DataReader::readNextEdge( void ) {
	if (_edgeFile != NULL && 
		fgets(_line, sizeof(_line), _edgeFile) != NULL) {
	
		Edge* e = new Edge();
		char* token = strtok(_line,",");

		int i = 0;
		while (token != NULL) {
			if (i == _edgeCols[X1]) {
				e->x1 = (float)atof(token);
			} else
			if (i == _edgeCols[Y1]) {
				e->y1 = (float)atof(token);
			} else
			if (i == _edgeCols[X2]) {
				e->x2 = (float)atof(token);
			} else
			if (i == _edgeCols[Y2]) {
				e->y2 = (float)atof(token);
			} else
			if (i == _edgeCols[E_WEIGHT]) {
				e->weight = atof(token);
			}						
		
			i++;
			token = strtok(NULL, ","); //next token
		}
		return e;
	} else {
		return NULL;
	}
}