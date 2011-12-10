#include "Working.hpp"
#include "../Renderer.hpp"
#include "WorkStateHelper.hpp"

Working::Working(Renderer* renderer) {
	_r = renderer;
	_visPainter = NULL;
	_worker = new WorkStateHelper(_r);
}

Working::~Working( void ) {
	delete _worker;
	delete _visPainter;
}

void Working::render( void ) {
	_r->renderGraph(_r->_currentData, _r->_mouseMoveX, _r->_mouseMoveY);
	_r->renderHUD(_worker->_progress);
}

void Working::renderGauss( void ) { /*do nothing*/ };

void Working::renderEvalField( void ) { /*do nothing*/ };

void Working::renderLineField( void ) { /*do nothing*/ };

void Working::work( void ) {
	if (!_worker->isDone())  {
		_worker->work();
	} else {
		//done change state
		//TMP UNTIL RESULTS WILL GO TO NEW INSTEAD OF CURRENT
			glDeleteTextures(1, &_r->_currentData->_gaussTex);
			if (_r->_hasEdges) {
				glDeleteTextures(1, &_r->_currentData->_evalField);
				glDeleteTextures(1, &_r->_currentData->_lineField);
				glDeleteTextures(1, &_r->_currentData->_vis);
			}
		//END TMP

		delete _visPainter;
		_visPainter = new VisPainter(_r->_windowWidth, _r->_windowHeight);

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