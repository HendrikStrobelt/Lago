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
	_r->renderGraph(_r->_currentData, _r->_mouseMoveX, _r->_mouseMoveY);
	
	float maxVals[2];
	maxVals[0] = _r->_currentData->_maxValuesN[2];
	maxVals[1] = _r->_currentData->_maxValuesE[1];
	_r->renderHUD(_worker->_progress, maxVals);
}

void Working::renderGauss( void ) { /*do nothing*/ };

void Working::renderEvalField( void ) { /*do nothing*/ };

void Working::renderLineField( void ) { /*do nothing*/ };

void Working::work( void ) {
	if (!_worker->isDone())  {
		_worker->work();
	} else {
		//done change state
		_r->_newData->_gaussTex = _worker->_pc[GAUSS_VIEW]->detachResult();

		if (_r->_hasEdges) {
			_r->_newData->_evalField = _worker->_fieldEvaluator[VIEW]->detachResultTexture();
			_r->_newData->_lineField = _worker->_linePainter->detachTexture();
		}

		_r->updateLabels(_r->_newData);
		_r->setState(_r->_visAdjust);
	}
}

void Working::takeOver( void ) {
	_r->_newData->clear();	
	_worker->takeOver();
}

void Working::changePanning(int xMouseMove, int yMouseMove) {
	_r->_mouseMoveX += xMouseMove;
	_r->_mouseMoveY += yMouseMove;
	takeOver();
}

void Working::changeZoom( void ) {
	takeOver();
}

void Working::changeData(string nodeFile, string edgeFile) {
	_r->setState(_r->_initalWork);
}

void Working::changeSideLength( void ) {
	takeOver();
}

void Working::changeWindow( void ) {
	_r->setState(_r->_initalWork);
}

void Working::changeVisParameter( void ) { /* do nothing */ }