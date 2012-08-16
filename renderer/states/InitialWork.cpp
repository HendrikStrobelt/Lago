#include "InitialWork.hpp"
#include "../Renderer.hpp"
#include "WorkStateHelper.hpp"
#include "../../context/Context.hpp"

#include <glm\glm.hpp>

InitialWork::InitialWork(Renderer* renderer) {
	_r = renderer;
	_worker = new WorkStateHelper(_r);
}

InitialWork::~InitialWork( void ) {
	delete _worker;
}

void InitialWork::render( void ) {
	int bars = 1;
	if (_r->_hasEdges) {
		bars = 3;
	}
	float maxVals[]  = {-1.0f, -1.0f};
	_r->renderHUD(_worker->_progress, bars, maxVals);
}

void InitialWork::renderGauss( void ) { /*do nothing*/ };

void InitialWork::renderEvalField( void ) { /*do nothing*/ };

void InitialWork::renderLineField( void ) { /*do nothing*/ };

void InitialWork::work( void ) {
	if (!_worker->isDone())  {
		_worker->work();
	} else {

		//done change state
		_r->_newData->setGaussTex(_worker->_pc[GAUSS_VIEW]->detachResult());
		_r->_newData->setEvalField(_worker->_fieldEvaluator[VIEW]->detachResultTexture());
		_r->_newData->setSideLength(context::_pixelSize * pow(SIDE_BASE, context::_sideExponent));
		_r->_newData->setSeedField(_worker->_gaussPainter[VIEW]->detachSeedTexture());

		glm::mat4 MVPI = glm::inverse(_r->getStandardMVP());
		glm::vec4 leftLow = MVPI * glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f);
		glm::vec4 rightUp = MVPI * glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
		float box[] = {leftLow.x, leftLow.y, rightUp.x, rightUp.y};
		_r->_newData->setBox(box);

		if (_r->_hasEdges) {	
			_r->_newData->setLineField(_worker->_linePainter->detachTexture());			
		}

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

void InitialWork::changeData( void ) {
	takeOver();
}

void InitialWork::changeSideLength( void ) {
	takeOver();
}

void InitialWork::changeWindow( void ) {
	takeOver();
}

void InitialWork::changeVisParameter( void ) { /* do nothing */ };
