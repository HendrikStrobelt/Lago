#include "Idle.hpp"
#include "../Renderer.hpp"

Idle::Idle(Renderer* renderer) {
	_r = renderer;
}

Idle::~Idle( void ) {

}

void Idle::render( void ) {
	_r->renderGraph(_r->_currentData);
}

void Idle::renderGauss( void ) {
	float* maxVals = _r->_currentData->_maxValuesN;
	_r->renderTexture(_r->_currentData->_gaussTex, maxVals[0], maxVals[1], maxVals[2]);
}

void Idle::renderEvalField( void ) {
	if (_r->_hasEdges) {
		float maxVals[3];
		_r->calculateMaxValues(maxVals, _r->_currentData->_evalField, _r->_windowWidth, _r->_windowHeight);
		_r->renderTexture(_r->_currentData->_evalField, maxVals[0], maxVals[1], maxVals[2]);
	}
}

void Idle::renderLineField( void ) {
	if (_r->_hasEdges) {
		float maxVals[3];
		_r->calculateMaxValues(maxVals, _r->_currentData->_lineField, _r->_windowWidth, _r->_windowHeight);
		_r->renderTexture(_r->_currentData->_lineField, maxVals[0], maxVals[1], maxVals[2]);
	}
}

void Idle::work( void ) { /*nothing to do*/ }

void Idle::takeOver( void ) {
	_r->_mouseMoveX = 0;
	_r->_mouseMoveY = 0;
}

void Idle::changePanning(int xMouseMove, int yMouseMove) {
	_r->_mouseMoveX = xMouseMove;
	_r->_mouseMoveY = yMouseMove;
	_r->setState(_r->_working);
}

void Idle::changeZoom( void ) {
	_r->setState(_r->_working);
}

void Idle::changeData(string nodeFile, string edgeFile) {
	_r->setState(_r->_initalWork);
}

void Idle::changeSideLength( void ) {
	_r->setState(_r->_working);
}

void Idle::changeWindow( void ) {
	_r->setState(_r->_initalWork);
}