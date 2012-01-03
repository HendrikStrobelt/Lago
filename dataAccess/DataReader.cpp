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


bool DataReader::hasNodeLabels( void ) {
	if (_nodeCols[N_LABEL] != NO_LABELS) {
		return true;
	} else {
		return false;
	}
}

bool DataReader::hasNodeLabelWeights( void ) {
	if (_nodeCols[N_LABEL_WEIGHT] != NO_LABEL_WEIGHTS) {
		return true;
	} else {
		return false;
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
	_nodeCols[N_LABEL_WEIGHT] = NO_LABEL_WEIGHTS;

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
				_nodeCols[N_WEIGHT] = i;
			} else 
			if (strcmp(token, "N_LABEL") == 0) {
				_nodeCols[N_LABEL] = i;
			} else 
			if (strcmp(token, "N_LABEL_WEIGHT") == 0) {
				_nodeCols[N_LABEL_WEIGHT] = i;
			}
		
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
			if (_nodeCols[N_LABEL_WEIGHT] != NO_LABEL_WEIGHTS) {
				cout << ", N_LABEL_WEIGHT(" << _nodeCols[N_LABEL_WEIGHT] << ")";
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

	_edgeCols[N1] = 0;
	_edgeCols[N2] = 1;
	_edgeCols[E_WEIGHT] = NO_WEIGHTS;

	fopen_s(&_edgeFile, filePath.c_str(), "r");
	if (_edgeFile) {
		fgets(_line, sizeof(_line), _edgeFile); //read first line
		bool foundN1 = false;
		bool foundN2 = false;
		
		int i = 0;
		char* token = strtok(_line,",");
		while (token != NULL) {
			if (strcmp(token, "N1") == 0) {
				_edgeCols[N1] = i;
				foundN1 = true;
			} else 
			if (strcmp(token, "N2") == 0) {
				_edgeCols[N2] = i;
				foundN2 = true;
			} else 
			if (strcmp(token, "E_WEIGHT") == 0) {
				_edgeCols[E_WEIGHT] = i;
			}

			i++;
			token = strtok(NULL, ","); //next token
		}

		if (foundN1 && foundN2) {
			cout << "header detection successfull: using N1(" << _edgeCols[N1] << "), N2(" << _edgeCols[N2] << ")";
			if (_edgeCols[E_WEIGHT] != NO_WEIGHTS) {
				cout << ", E_WEIGHT(" << _edgeCols[E_WEIGHT] << ")";
			}
			cout << "\n";
		} else {
			cout << "no header detected: skipping line1 using fallback N1(0), N2(1)" << "\n";
		}
		_edgeFileSet = true;
	} else {
		_edgeFileSet = false;
		_edgeFile = NULL;
		cerr << "ERROR: file open failed on " << filePath << "\n";
	}
}


bool DataReader::readNextNode(vector<Node>* nodes, vector<Label>* labels) {
	if (_nodeFileSet && 
		fgets(_line, sizeof(_line), _nodeFile) != NULL) {

		Node n;
		Label l;

		char* token = strtok(_line,",\n");

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
				strncpy(l.text, token, MAX_LABEL_LENGTH);
				l.text[MAX_LABEL_LENGTH] = '\0';
			} else 
			if (i == _nodeCols[N_LABEL_WEIGHT]) {
				l.weight = (float)atof(token);
			}						
		
			i++;
			token = strtok(NULL, ",\n"); //next token
		}
		
		if (hasNodeLabels() && labels != NULL) {
			l.x = n.x;
			l.y = n.y;
			l.id = labels->size();
			n.labelID = labels->size();
			labels->push_back(l);
		}
		nodes->push_back(n);
		return true;
	} else {
		return false;
	}
}

bool DataReader::readNextEdge(vector<ReferenceEdge>* edges) {
	if (_edgeFileSet && 
		fgets(_line, sizeof(_line), _edgeFile) != NULL) {
	
		ReferenceEdge e;
		char* token = strtok(_line,",\n");

		int i = 0;
		while (token != NULL) {
			if (i == _edgeCols[N1]) {
				e.n1Ref = (int)atoi(token);
			} else
			if (i == _edgeCols[N2]) {
				e.n2Ref = (int)atoi(token);
			} else
			if (i == _edgeCols[E_WEIGHT]) {
				e.weight = atof(token);
			}						
		
			i++;
			token = strtok(NULL, ",\n"); //next token
		}

		edges->push_back(e);
		return true;
	} else {
		return false;
	}
}