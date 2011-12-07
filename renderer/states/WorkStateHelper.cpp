#include "WorkStateHelper.hpp"

#include "../../helper/CameraHelper.hpp"
#include "../../context/Context.hpp"
#include "../../GlobalConstants.hpp"

#include <glm/gtc/matrix_transform.hpp>

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

	int joinDepth = _r->dCache.getNodeStructureInfo()->getJoinDepth(context::_pixelSize);
	int elementCount = _r->dCache.getNodeStructureInfo()->getAllNodes(joinDepth);
   	_gaussPainter[VIEW] = new GaussPainter(_r->_nodeVBO, elementCount);
	
	glm::mat4 P = cameraHelper::calculateProjection(_r->dCache.getNodeStructureInfo(), context::_zoomFactor);
	glm::mat4 MVP = glm::translate(P, glm::vec3(context::_worldTransX, context::_worldTransY, 0.0f));

	float sideLength = context::_pixelSize * pow(SIDE_BASE, context::_sideExponent);
	_gaussPainter[VIEW]->setBaseVars(MVP, sideLength, joinDepth);
	
	_pc[GAUSS_VIEW] = new PainterCommander(_gaussPainter[VIEW], _r->_windowWidth, _r->_windowHeight, POINT_INIT_STEP);
}