#include "Options.hpp"
#include <sstream>
#include "Context.hpp"
#include "../helper/EnvironmentHelper.hpp"

using std::boolalpha;

Options::Options( void ) {
	_nodeFile = "";
	_edgeFile = "";
	_colorSchemeNode = "node.tga";
	_colorSchemeEdge = "edge.tga";
	_colorSchemeLabel = "label.tga";
	_nodeScheme = -1;
	_antiAlias = true;
	_adaptiveScaleBars = true;
	_showLabels = true;
	_labelCount = START_MAX_LABELS;
}

void Options::init( void ) {
	loadTextures();
}

Options::~Options( void ) {
	glDeleteTextures(1, &_nodeScheme);
	glDeleteTextures(1, &_edgeScheme);
	glDeleteTextures(1, &_labelScheme);
}


string Options::toCommandString( void ) {
	stringstream s;
	s << " antiAlias$" << boolalpha <<  _antiAlias;
	s << " adaptiveScaleBars$" << boolalpha << _adaptiveScaleBars;
	s << " nodeFile$" << "f." << _nodeFile;
	s << " edgeFile$" << "f." << _edgeFile;
	s << " nodeCS$" << _colorSchemeNode;
	s << " edgeCS$" << _colorSchemeEdge;
	s << " labelCS$" << _colorSchemeLabel;
	s << " showLabels$" << _showLabels;
	s << " labelCount$" << _labelCount;
	return s.str();
}

void Options::update(map<string, string> dataMap) {
	if (!_changedLocal) {
		map<string, string>::iterator it;

		//HUD 
		if ((it = dataMap.find("adaptiveScaleBars")) != dataMap.end()) {
			if (it->second.compare("true") == 0) {
				_adaptiveScaleBars = true;
			} else {
				_adaptiveScaleBars = false;
			}
		}


		//VIS PARAMETER
		bool visChange = false;

		if ((it = dataMap.find("antiAlias")) != dataMap.end()) {
			bool aa;
			if (it->second.compare("true") == 0) {
				aa = true;
			} else {
				aa = false;
			}
			if (aa != _antiAlias) {
				visChange = true;
				_antiAlias = aa;
			}
		}

		if ((it = dataMap.find("showLabels")) != dataMap.end()) {
			bool showLabels;
			if (it->second.compare("true") == 0) {
				showLabels = true;
			} else {
				showLabels = false;
			}
			if (showLabels != _showLabels) {
				if (showLabels) {//label update is only necessary if we show them
					visChange = true; 
				}
				_showLabels = showLabels;
			}
		}

		if ((it = dataMap.find("labelCount")) != dataMap.end()) {
			int labelCount;
			labelCount = atoi(it->second.c_str());
			if (labelCount != _labelCount) {
				if (_showLabels) {//label update is only necessary if we show them
					visChange = true;
				}
				_labelCount = labelCount;
			}
		}

		if (visChange) {
			context::visParameterChange();
		}


		//DATA SET
		string nFile, eFile;
		if ((it = dataMap.find("nodeFile")) != dataMap.end()) {
			nFile = it->second.substr(2);
		}

		if ((it = dataMap.find("edgeFile")) != dataMap.end()) {
			eFile = it->second.substr(2);
		}

		if (!nFile.empty() && (nFile != _nodeFile || eFile != _edgeFile)) {
			_nodeFile = nFile;
			_edgeFile = eFile;
			context::setDataSet(_nodeFile, _edgeFile);
		}

		//COLOR SCHEMES
		string csNode, csEdge, csLabel;
		if ((it = dataMap.find("nodeCS")) != dataMap.end()) {
			csNode = it->second;
		}

		if ((it = dataMap.find("edgeCS")) != dataMap.end()) {
			csEdge = it->second;
		}

		if ((it = dataMap.find("labelCS")) != dataMap.end()) {
			csLabel = it->second;
		}

		if (!csLabel.empty() && (csLabel != _colorSchemeLabel)) {
			_colorSchemeLabel = csLabel;
			loadTextures();
		}

		if (   (!csEdge.empty() && !csNode.empty()) && (csEdge != _colorSchemeEdge || csNode != _colorSchemeNode)  ) {
			_colorSchemeEdge = csEdge;
			_colorSchemeNode = csNode;
			loadTextures();
			context::visParameterChange();
		}

	}
}

void Options::dataChanged(string nodeFile, string edgeFile) {
	_nodeFile = nodeFile;
	_edgeFile = edgeFile;
	_changedLocal = true;
	context::setDataSet(_nodeFile, _edgeFile);
}

void Options::loadTextures( void ) {
	if (_nodeScheme != -1) {
		glDeleteTextures(1, &_nodeScheme);
		glDeleteTextures(1, &_edgeScheme);
		glDeleteTextures(1, &_labelScheme);
	}

	string path("_Tex//colorSchemes//");

	string node = path + _colorSchemeNode;
	_nodeScheme = envHelper::loadRGBTexture(node);

	string edge = path + _colorSchemeEdge;
	_edgeScheme = envHelper::loadRGBTexture(edge);

	string label = path + _colorSchemeLabel;
	_labelScheme = envHelper::loadRGBTexture(label);
}

void Options::keyEvent(int key, int action) {
	if (action == GLFW_RELEASE) {
		if (key == '1') {
			dataChanged("_Data//LineNode.out");
		} else 
		if (key == '2') {
			dataChanged("_Data//GridNode.out", "_Data//GridEdge.out");
		} else 
		if (key == '3') {
			dataChanged("_Data//WorkNode.out", "_Data//WorkEdge.out");
		} else 
		if (key == '4') {
			dataChanged("_Data//GermanyNode.out");
		}
	}
}