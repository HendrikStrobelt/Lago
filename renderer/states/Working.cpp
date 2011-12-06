#include "Working.hpp"
#include "../Renderer.hpp"
#include "WorkStateHelper.hpp"

Working::Working(Renderer* renderer) {
	_r = renderer;
	_worker = new WorkStateHelper(_r);
}

Working::~Working( void ) {
	delete _worker;
}

void Working::render( void ) {
	float* maxVals = _r->_currentData->_maxValuesN;
	_r->renderTexture(_r->_currentData->_gaussTex, maxVals[0], maxVals[1], maxVals[2]);
	_r->renderHUD(_worker->_progress);
}

void Working::renderGauss( void ) {};
void Working::renderEvalField( void ) {};

void Working::work( void ) {
	if (!_worker->_pc[GAUSS_VIEW]->isDone())  {
		_worker->work();
	} else {
		//done change state
		_r->_currentData->_gaussTex = _worker->_pc[GAUSS_VIEW]->detachResult();
		_r->calculateMaxValues(_r->_currentData->_maxValuesN, _r->_currentData->_gaussTex, _r->_windowWidth, _r->_windowHeight);

		_r->setState(_r->_idle);
	}
}

void Working::takeOver( void ) {
	_r->_newData->clear();
	
	_worker->takeOver();
}

void Working::changePanning( void ) {};
void Working::changeZoom( void ) {};

void Working::changeData(string nodeFile, string edgeFile) {
	_r->setState(_r->_initalWork);
}

void Working::changeSideRatio( void ) {
	takeOver();
}