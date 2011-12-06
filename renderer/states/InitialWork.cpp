#include "InitialWork.hpp"
#include "../Renderer.hpp"
#include "../../GlobalConstants.hpp"

#include <glm/gtc/matrix_transform.hpp>

InitialWork::InitialWork(Renderer* renderer) {
	_r = renderer;
	_gaussPainter[VIEW] = NULL;
	_pc[GAUSS_VIEW] = NULL;
	_progress = 0.0f;
}

InitialWork::~InitialWork( void ) {
	delete _gaussPainter[VIEW];
	delete _pc[GAUSS_VIEW];
}

void InitialWork::render( void ) {
	_r->renderHUD(_progress);
}

void InitialWork::renderGauss( void ) {
	
}

void InitialWork::renderEvalField( void ) {

}


void InitialWork::work( void ) {
	if (!_pc[GAUSS_VIEW]->isDone())  {
		_progress = _pc[GAUSS_VIEW]->renderNextPart();
	} else {
		//done change state
		_r->_currentData->clear();
		_r->_currentData->_gaussTex = _pc[GAUSS_VIEW]->detachResult();
		_r->calculateMaxValues(_r->_currentData->_maxValuesN, _r->_currentData->_gaussTex, _r->_windowWidth, _r->_windowHeight);

		_r->_state = _r->_idle;
	}
}


void InitialWork::changePanning( void ) {

}

void InitialWork::changeZoom( void ) {

}

void InitialWork::changeData( void ) {
	int elementCount = _r->dCache.getNodeStrucutreInfo()->getAllNodes(_r->dCache.getNodeStrucutreInfo()->getMaxDepth());
   	
	delete _gaussPainter[VIEW];
	_gaussPainter[VIEW] = new GaussPainter(_r->_nodeVBO, elementCount);
    _gaussPainter[VIEW]->setBaseVars(glm::ortho<float>(0, 36335, -41125, 0, -1, 1), 614.0f, _r->dCache.getNodeStrucutreInfo()->getMaxDepth());
	delete _pc[GAUSS_VIEW];    
	_pc[GAUSS_VIEW] = new PainterCommander(_gaussPainter[VIEW], _r->_windowWidth, _r->_windowHeight, POINT_INIT_STEP);
}


void InitialWork::changeAspectRatio( void ) {

}