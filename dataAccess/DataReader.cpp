#include "DataReader.hpp"

DataReader::DataReader( void ) {
	_nodeFile = NULL;
	_edgeFile = NULL;
	_nodeFileSet = false;
	_edgeFileSet = false;
}

DataReader::~DataReader( void ) {
	if (_nodeFile != NULL) {
		fclose(_nodeFile);
	}
	if (_edgeFile != NULL) {
		fclose(_edgeFile);
	}
}



void DataReader::setNodeFile(string filePath) {
	if (_nodeFile != NULL) {
		fclose(_nodeFile);
	}

	_nodeCols[X] = 0;
	_nodeCols[Y] = 1;
	_nodeCols[N_WEIGHT] = NO_WEIGHTS;
	_nodeCols[N_LABEL] = NO_LABELS;

	fopen_s(&_nodeFile, filePath.c_str(), "r");
	if (_nodeFile) {
		fgets(_line, sizeof(_line), _nodeFile); //read first line
		bool foundX = false;
		bool foundY = false;
		
		int i = 0;
		char* token = strtok(_line,",");
		while (token != NULL) {
			if (strcmp(token, "X") == 0) {
				_nodeCols[X] = i;
				foundX = true;
			} else 
			if (strcmp(token, "Y") == 0) {
				_nodeCols[Y] = i;
				foundY = true;
			} else 
			if (strcmp(token, "N_WEIGHT") == 0) {
				_nodeCols[Y] = i;
			} else 
			if (strcmp(token, "N_LABEL") == 0) {
				_nodeCols[Y] = i;
			};
		
			i++;
			token = strtok(NULL, ","); //next token
		}

		if (foundX && foundY) {
			cout << "header detection successfull: using X(" << _nodeCols[X] << "), Y(" << _nodeCols[Y] << ")";
			if (_nodeCols[N_WEIGHT] != NO_WEIGHTS) {
				cout << ", N_WEIGHT(" << _nodeCols[N_WEIGHT] << ")";
			}
			if (_nodeCols[N_LABEL] != NO_LABELS) {
				cout << ", N_LABEL(" << _nodeCols[N_LABEL] << ")";
			}
			cout << "\n";
		} else {
			cout << "no header detected: skipping line1 using fallback X(0), Y(1)" << "\n";
		}
		_nodeFileSet = true;
	} else {
		_nodeFileSet = false;
		_nodeFile = NULL;
		cerr << "ERROR: file open failed on " << filePath << "\n";
	}
}


void DataReader::setEdgeFile(string filePath) {
	if (_edgeFile != NULL) {
		fclose(_edgeFile);
	}

	_edgeCols[X1] = 0;
	_edgeCols[Y1] = 1;
	_edgeCols[X2] = 2;
	_edgeCols[Y2] = 3;
	_edgeCols[E_WEIGHT] = NO_WEIGHTS;

	fopen_s(&_edgeFile, filePath.c_str(), "r");
	if (_edgeFile) {
		fgets(_line, sizeof(_line), _edgeFile); //read first line
		bool foundX1 = false;
		bool foundY1 = false;
		bool foundX2 = false;
		bool foundY2 = false;
		
		int i = 0;
		char* token = strtok(_line,",");
		while (token != NULL) {
			if (strcmp(token, "X1") == 0) {
				_edgeCols[X1] = i;
				foundX1 = true;
			} else 
			if (strcmp(token, "Y1") == 0) {
				_edgeCols[Y1] = i;
				foundY1 = true;
			} else 
			if (strcmp(token, "X2") == 0) {
				_edgeCols[X2] = i;
				foundX2 = true;
			} else 
			if (strcmp(token, "Y2") == 0) {
				_edgeCols[Y2] = i;
				foundY2 = true;
			} else 
			if (strcmp(token, "E_WEIGHT") == 0) {
				_edgeCols[E_WEIGHT] = i;
			}

			i++;
			token = strtok(NULL, ","); //next token
		}

		if (foundX1 && foundY1 && foundX2 && foundY2) {
			cout << "header detection successfull: using X1(" << _edgeCols[X1] << "), Y1(" << _edgeCols[Y1] << ")";
			cout << ", X2(" << _edgeCols[X2] << "), Y2(" << _edgeCols[Y2] << ")";
			if (_edgeCols[E_WEIGHT] != NO_WEIGHTS) {
				cout << ", E_WEIGHT(" << _edgeCols[E_WEIGHT] << ")";
			}
			cout << "\n";
		} else {
			cout << "no header detected: skipping line1 using fallback X1(0), Y1(1), X2(2), Y2(3)" << "\n";
		}
		_edgeFileSet = true;
	} else {
		_edgeFileSet = false;
		_edgeFile = NULL;
		cerr << "ERROR: file open failed on " << filePath << "\n";
	}
}


bool DataReader::readNextNode(vector<Node>* nodes) {
	if (_nodeFileSet && 
		fgets(_line, sizeof(_line), _nodeFile) != NULL) {

		Node n;
		char* token = strtok(_line,",");

		int i = 0;
		while (token != NULL) {
			if (i == _nodeCols[X]) {
				n.x = (float)atof(token);
			} else
			if (i == _nodeCols[Y]) {
				n.y = (float)atof(token);
			} else
			if (i == _nodeCols[N_WEIGHT]) {
				n.weight = (float)atof(token);
			} else 
			if (i == _nodeCols[N_LABEL]) {
				//TODO not yet supported node labels
			}						
		
			i++;
			token = strtok(NULL, ","); //next token
		}
		
		nodes->push_back(n);
		return true;
	} else {
		return false;
	}
}

bool DataReader::readNextEdge(vector<Edge>* edges) {
	if (_edgeFileSet && 
		fgets(_line, sizeof(_line), _edgeFile) != NULL) {
	
		Edge e;
		char* token = strtok(_line,",");

		int i = 0;
		while (token != NULL) {
			if (i == _edgeCols[X1]) {
				e.x1 = (float)atof(token);
			} else
			if (i == _edgeCols[Y1]) {
				e.y1 = (float)atof(token);
			} else
			if (i == _edgeCols[X2]) {
				e.x2 = (float)atof(token);
			} else
			if (i == _edgeCols[Y2]) {
				e.y2 = (float)atof(token);
			} else
			if (i == _edgeCols[E_WEIGHT]) {
				e.weight = atof(token);
			}						
		
			i++;
			token = strtok(NULL, ","); //next token
		}

		edges->push_back(e);
		return true;
	} else {
		return false;
	}
}