#include "InitialWork.hpp"
#include "../Renderer.hpp"
#include "WorkStateHelper.hpp"

#include <glm\glm.hpp>

InitialWork::InitialWork(Renderer* renderer) {
	_r = renderer;
	_visPainter = NULL;
	_worker = new WorkStateHelper(_r);
}

InitialWork::~InitialWork( void ) {
	delete _worker;
	delete _visPainter;
}

void InitialWork::render( void ) {
	_r->renderHUD(_worker->_progress);
}

void InitialWork::renderGauss( void ) { /*do nothing*/ };

void InitialWork::renderEvalField( void ) { /*do nothing*/ };

void InitialWork::renderLineField( void ) { /*do nothing*/ };

void InitialWork::work( void ) {
	if (!_worker->isDone())  {
		_worker->work();
	} else {
		delete _visPainter;
		_visPainter = new VisPainter(_r->_windowWidth, _r->_windowHeight);

		//done change state
		_r->_currentData->_gaussTex = _worker->_pc[GAUSS_VIEW]->detachResult();
		_r->_currentData->_maxValuesN[0] = _worker->_nodeMax[0];
		_r->_currentData->_maxValuesN[1] = _worker->_nodeMax[1];
		_r->_currentData->_maxValuesN[2] = _worker->_nodeMax[2];

		if (_r->_hasEdges) {
			_r->_currentData->_evalField = _worker->_fieldEvaluator[VIEW]->detachResultTexture();
			_r->_currentData->_lineField = _worker->_linePainter->detachTexture();			
			_r->_currentData->_maxValuesE[0] = _worker->_edgeMax[0];
			_r->_currentData->_maxValuesE[1] = _worker->_edgeMax[1];
			_r->_currentData->_maxValuesE[2] = _worker->_edgeMax[2];
		}

		_visPainter->renderVis(_r->_currentData, _r->_hasEdges);
		_r->_currentData->_vis = _visPainter->detachResult();

		_r->setState(_r->_idle);
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