#include "VisAdjusting.hpp"
#include "../Renderer.hpp"

VisAdjusting::VisAdjusting(Renderer* renderer) {
	_r = renderer;
	_visPainter = NULL;
}

VisAdjusting::~VisAdjusting( void ) {
	delete _visPainter;
}


//Interface methods

void VisAdjusting::render( void ) {
	_r->renderGraph(_r->_newData, 0.0f, 0.0f);
	
	float maxVals[2];
	maxVals[0] = _r->_newData->_maxValuesN[2];
	maxVals[1] = _r->_newData->_maxValuesE[1];
	_r->renderHUD(-1.0f, maxVals);
	_r->renderLabelSelection(_r->_newData);
}

void VisAdjusting::renderGauss( void ) { /* nothing to do */ }
void VisAdjusting::renderEvalField( void ) { /* nothing to do */ }
void VisAdjusting::renderLineField( void ) { /* nothing to do */ }

void VisAdjusting::work( void ) {

	//done
	swap();
	_r->setState(_r->_idle);
}

void VisAdjusting::takeOver( void ) {
	_r->_mouseMoveX = 0;
	_r->_mouseMoveY = 0;

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

	_r->calculateMaxValues(_r->_newData->_maxValuesN, _r->_newData->_gaussTex, _r->_windowWidth, _r->_windowHeight);
	_r->calculateMaxValues(_r->_newData->_maxValuesE, _r->_newData->_lineField, _r->_windowWidth, _r->_windowHeight);

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