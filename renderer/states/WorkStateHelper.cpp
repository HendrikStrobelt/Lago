#include "WorkStateHelper.hpp"

#include "../../helper/CameraHelper.hpp"
#include "../../context/Context.hpp"
#include "../../GlobalConstants.hpp"

WorkStateHelper::WorkStateHelper(Renderer* renderer) {
	_r = renderer;

	_gaussPainter[VIEW] = NULL;
	_pc[GAUSS_VIEW] = NULL;
	_progress = 0.0f;
}	

WorkStateHelper::~WorkStateHelper( void ) {
	delete _gaussPainter[VIEW];
	delete _pc[GAUSS_VIEW];
}


void WorkStateHelper::work( void ) {
	_progress = _pc[GAUSS_VIEW]->renderNextPart();
}

void WorkStateHelper::takeOver( void ) {
	_progress = 0.0f;

	delete _gaussPainter[VIEW];
	delete _pc[GAUSS_VIEW];

	int elementCount = _r->dCache.getNodeStructureInfo()->getAllNodes(_r->dCache.getNodeStructureInfo()->getMaxDepth());
   	_gaussPainter[VIEW] = new GaussPainter(_r->_nodeVBO, elementCount);
	
	glm::mat4 P = cameraHelper::calculateProjection(_r->dCache.getNodeStructureInfo(), context::_userZoomFactor);
	float sideLength = GaussPainter::getQuadSideLength(_r->dCache.getNodeStructureInfo()->getWidth(), context::_sideRatio);
    _gaussPainter[VIEW]->setBaseVars(P, sideLength, _r->dCache.getNodeStructureInfo()->getMaxDepth());
	
	_pc[GAUSS_VIEW] = new PainterCommander(_gaussPainter[VIEW], _r->_windowWidth, _r->_windowHeight, POINT_INIT_STEP);
}