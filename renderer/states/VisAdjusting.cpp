#include "VisAdjusting.hpp"
#include "../Renderer.hpp"
#include "../../GlobalConstants.hpp"

VisAdjusting::VisAdjusting(Renderer* renderer) {
	_r = renderer;
	_visPainter = NULL;
	_blendVis = -1;
	_blendRenderData = NULL;
}

VisAdjusting::~VisAdjusting( void ) {
	delete _visPainter;
	delete _blendRenderData;
	if (_blendVis != -1) {
		glDeleteTextures(1, &_blendVis);
	}
}


//Interface methods

void VisAdjusting::render( void ) {
	_r->renderGraph(_blendRenderData, _r->_mouseMoveX, _r->_mouseMoveY);
}

void VisAdjusting::renderGauss( void ) { /* nothing to do */ }
void VisAdjusting::renderEvalField( void ) { /* nothing to do */ }
void VisAdjusting::renderLineField( void ) { /* nothing to do */ }

void VisAdjusting::work( void ) {
	if (COLOR_BLEND_ON && _blendChange && _blendFactor < 1.0f) {
		float maxN = _r->_currentData->_maxValuesN[2] * (1.0f - _blendFactor) + _r->_newData->_maxValuesN[2] * _blendFactor;
		float maxE = _r->_currentData->_maxValuesE[1] * (1.0f - _blendFactor) + _r->_newData->_maxValuesE[1] * _blendFactor;
		
		delete _blendRenderData; //blend render data is a shadow copy most things will not be deleted
		_blendRenderData = new RenderData(_r->_newData); //create a shadow copy
		_blendRenderData->_maxValuesN[2] = maxN; //no pointers will not reflect to _newData
		_blendRenderData->_maxValuesE[1] = maxE;

		delete _visPainter;
		_visPainter = new VisPainter(_r->_windowWidth, _r->_windowHeight);
		_visPainter->renderVis(_blendRenderData, _r->_hasEdges);

		if (_blendVis != -1) {
			glDeleteTextures(1, &_blendVis);
		}
		_blendVis = _visPainter->detachResult();

		_blendRenderData->_vis = _blendVis;

		_blendFactor += 0.01;
	} else {
		//done
		swap();
		_r->setState(_r->_idle);
	}
}

void VisAdjusting::takeOver( void ) {
	_r->_mouseMoveX = 0;
	_r->_mouseMoveY = 0;
	_blendFactor = 0.0f;
	_blendChange = false;

	delete _blendRenderData;
	_blendRenderData = new RenderData(_r->_currentData); //just a shadow copy blend render data will not delete the textures

	//create valid set data in slot newData
	if (_r->_newData->_gaussTex == -1) {
		//new data is not complete steal from _currentData
		_r->_newData->_evalField = _r->_currentData->_evalField;
		_r->_currentData->_evalField = -1;
		_r->_newData->_gaussTex = _r->_currentData->_gaussTex;
		_r->_currentData->_gaussTex = -1;
		_r->_newData->_lineField = _r->_currentData->_lineField;
		_r->_currentData->_lineField = -1;
	}

	//adapt max values

	float nodeValues[3];
	_r->calculateMaxValues(nodeValues, _r->_newData->_gaussTex, _r->_windowWidth, _r->_windowHeight);
	if (abs(nodeValues[2] - _r->_currentData->_maxValuesN[2]) > (_r->_currentData->_maxValuesN[2] * BLEND_MARGIN)) {
		_blendChange = true;
		_r->_newData->_maxValuesN[0] = nodeValues[0];
		_r->_newData->_maxValuesN[1] = nodeValues[1];
		_r->_newData->_maxValuesN[2] = nodeValues[2];
	} else {
		_r->_newData->_maxValuesN[0] = _r->_currentData->_maxValuesN[0];
		_r->_newData->_maxValuesN[1] = _r->_currentData->_maxValuesN[1];
		_r->_newData->_maxValuesN[2] = _r->_currentData->_maxValuesN[2];
	}

	float edgeValues[3];
	_r->calculateMaxValues(edgeValues, _r->_newData->_lineField, _r->_windowWidth, _r->_windowHeight);
	if (abs(edgeValues[1] - _r->_currentData->_maxValuesE[1]) > (_r->_currentData->_maxValuesE[1] * BLEND_MARGIN)) {
		_blendChange = true;
		_r->_newData->_maxValuesE[0] = edgeValues[0];
		_r->_newData->_maxValuesE[1] = edgeValues[1];
		_r->_newData->_maxValuesE[2] = edgeValues[2];
	} else {
		_r->_newData->_maxValuesE[0] = _r->_currentData->_maxValuesE[0];
		_r->_newData->_maxValuesE[1] = _r->_currentData->_maxValuesE[1];
		_r->_newData->_maxValuesE[2] = _r->_currentData->_maxValuesE[2];
	}


	delete _visPainter;
	_visPainter = new VisPainter(_r->_windowWidth, _r->_windowHeight);
	_visPainter->renderVis(_r->_newData, _r->_hasEdges);
	_r->_newData->_vis = _visPainter->detachResult();
}

void VisAdjusting::changePanning(int xMouseMove, int yMouseMove) {
	_r->_mouseMoveX = xMouseMove;
	_r->_mouseMoveY = yMouseMove;
	swap();
	_r->setState(_r->_working);
}

void VisAdjusting::changeZoom( void ) {
	swap();
	_r->setState(_r->_working);
}

void VisAdjusting::changeData(string nodeFile, string edgeFile) {
	//double clear no swap necessary
	_r->setState(_r->_initalWork);
}

void VisAdjusting::changeSideLength( void ) {
	swap();
	_r->setState(_r->_working);
}

void VisAdjusting::changeWindow( void ) {
	//double clear no swap necessary
	_r->setState(_r->_initalWork);
}

void VisAdjusting::changeVisParameter( void ) {
	takeOver();
}


//private stuff
void VisAdjusting::swap( void ) {
   	_r->_currentData->clear();
   	RenderData* swap = _r->_currentData;
   	_r->_currentData = _r->_newData;
   	_r->_newData = swap;
}