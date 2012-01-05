#include "Idle.hpp"
#include "../Renderer.hpp"

Idle::Idle(Renderer* renderer) {
	_r = renderer;
}

Idle::~Idle( void ) {

}

void Idle::render( void ) {
	_r->renderGraph(_r->_currentData);
	
	float maxVals[2];
	maxVals[0] = _r->_currentData->getNodeMax();
	maxVals[1] = _r->_currentData->getEdgeMax();
	_r->renderHUD(-1.0f, maxVals);
	_r->renderLabelSelection(_r->_currentData);
}

void Idle::renderGauss( void ) {
	float* maxVals = _r->_currentData->getNodeMaxAll();
	_r->renderTexture(_r->_currentData->getGaussTex(), maxVals);
}

void Idle::renderEvalField( void ) {
	float maxVals[3];
	_r->calculateMaxValues(maxVals, _r->_currentData->getEvalField(), _r->_windowWidth, _r->_windowHeight);
	_r->renderTexture(_r->_currentData->getEvalField(), maxVals);
}

void Idle::renderLineField( void ) {
	if (_r->_hasEdges) {
		float maxVals[3];
		_r->calculateMaxValues(maxVals, _r->_currentData->getLineField(), _r->_windowWidth, _r->_windowHeight);
		_r->renderTexture(_r->_currentData->getLineField(), maxVals);
	}
}

void Idle::work( void ) { /*nothing to do*/ }

void Idle::takeOver( void ) { /*nothing to do*/ }

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

void Idle::changeVisParameter( void ) {
	_r->setState(_r->_visAdjust);
}