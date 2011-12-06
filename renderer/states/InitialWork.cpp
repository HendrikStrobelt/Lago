#include "InitialWork.hpp"
#include "../Renderer.hpp"
#include "../../GlobalConstants.hpp"
#include "../../helper/CameraHelper.hpp"
#include "../../context/Context.hpp"

#include <glm\glm.hpp>

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
		_r->_currentData->_gaussTex = _pc[GAUSS_VIEW]->detachResult();
		_r->calculateMaxValues(_r->_currentData->_maxValuesN, _r->_currentData->_gaussTex, _r->_windowWidth, _r->_windowHeight);

		_r->_state = _r->_idle;
	}
}

void InitialWork::takeOver( void ) {
	_r->_currentData->clear();
	_r->_newData->clear();

	delete _gaussPainter[VIEW];
	delete _pc[GAUSS_VIEW];

	int elementCount = _r->dCache.getNodeStructureInfo()->getAllNodes(_r->dCache.getNodeStructureInfo()->getMaxDepth());
   	_gaussPainter[VIEW] = new GaussPainter(_r->_nodeVBO, elementCount);
	
	glm::mat4 P = cameraHelper::calculateProjection(_r->dCache.getNodeStructureInfo(), 1.0f);
	float sideLength = GaussPainter::getQuadSideLength(_r->dCache.getNodeStructureInfo()->getWidth(), context::_sideRatio);
    _gaussPainter[VIEW]->setBaseVars(P, sideLength, _r->dCache.getNodeStructureInfo()->getMaxDepth());
	
	_pc[GAUSS_VIEW] = new PainterCommander(_gaussPainter[VIEW], _r->_windowWidth, _r->_windowHeight, POINT_INIT_STEP);
}


void InitialWork::changePanning( void ) {

}

void InitialWork::changeZoom( void ) {

}

void InitialWork::changeData(string nodeFile, string edgeFile) {
	_r->setState(_r->_initalWork);
}


void InitialWork::changeAspectRatio( void ) {

}