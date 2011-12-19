#include "InitialWork.hpp"
#include "../Renderer.hpp"
#include "WorkStateHelper.hpp"

#include <glm\glm.hpp>

InitialWork::InitialWork(Renderer* renderer) {
	_r = renderer;
	_worker = new WorkStateHelper(_r);
}

InitialWork::~InitialWork( void ) {
	delete _worker;
}

void InitialWork::render( void ) {
	float maxVals[]  = {-1.0f, -1.0f};
	_r->renderHUD(_worker->_progress, maxVals);
}

void InitialWork::renderGauss( void ) { /*do nothing*/ };

void InitialWork::renderEvalField( void ) { /*do nothing*/ };

void InitialWork::renderLineField( void ) { /*do nothing*/ };

void InitialWork::work( void ) {
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

void InitialWork::takeOver( void ) {
	_r->_currentData->clear();
	_r->_newData->clear();
	_r->_mouseMoveX = 0;
	_r->_mouseMoveY = 0;
	
	_worker->takeOver();
}


void InitialWork::changePanning(int xMouseMove, int yMouseMove) {
	takeOver();
}

void InitialWork::changeZoom( void ) {
	takeOver();
}

void InitialWork::changeData(string nodeFile, string edgeFile) {
	takeOver();
}

void InitialWork::changeSideLength( void ) {
	takeOver();
}

void InitialWork::changeWindow( void ) {
	takeOver();
}

void InitialWork::changeVisParameter( void ) { /* do nothing */ };
