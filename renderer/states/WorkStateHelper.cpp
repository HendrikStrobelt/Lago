#include "WorkStateHelper.hpp"

#include "../../helper/CameraHelper.hpp"
#include "../../context/Context.hpp"
#include "../../GlobalConstants.hpp"

#include <glm/gtc/matrix_transform.hpp>

WorkStateHelper::WorkStateHelper(Renderer* renderer) {
	_r = renderer;

	_gaussPainter[VIEW] = NULL;
	_pc[GAUSS_VIEW] = NULL;
	_fieldEvaluator = NULL;
	_progress = 0.0f;
}	

WorkStateHelper::~WorkStateHelper( void ) {
	delete _gaussPainter[VIEW];
	delete _pc[GAUSS_VIEW];
	delete _fieldEvaluator;
}


bool WorkStateHelper::isDone( void ) {
	if (_r->_hasEdges) {
		return (_pc[GAUSS_VIEW]->isDone() &&
			    _fieldEvaluator->isDone());
	} else {
		return _pc[GAUSS_VIEW]->isDone();
	}
}

void WorkStateHelper::work( void ) {
	if (_r->_hasEdges) {
		if (!_pc[GAUSS_VIEW]->isDone()) {
			_progress = _pc[GAUSS_VIEW]->renderNextPart();
		} else 
		if (!_fieldEvaluator->isDone()) {
			_fieldEvaluator->evaluate(_pc[GAUSS_VIEW]->getWorkingTexture());
			_progress = 0.5f;
		} else {
			_progress = 1.0f;
		}
	} else {
		_progress = _pc[GAUSS_VIEW]->renderNextPart();
	}
}

void WorkStateHelper::takeOver( void ) {
	_progress = 0.0f;

	delete _gaussPainter[VIEW];
	delete _pc[GAUSS_VIEW];
	delete _fieldEvaluator;

	int joinDepth = _r->dCache.getNodeStructureInfo()->getJoinDepth(context::_pixelSize);
	int elementCount = _r->dCache.getNodeStructureInfo()->getAllNodes(joinDepth);
   	_gaussPainter[VIEW] = new GaussPainter(_r->_nodeVBO, _r->_windowWidth, _r->_windowHeight, elementCount);
	
	glm::mat4 P = cameraHelper::calculateProjection(_r->dCache.getNodeStructureInfo(), context::_zoomFactor);
	glm::mat4 MVP = glm::translate(P, glm::vec3(context::_worldTransX, context::_worldTransY, 0.0f));

	float sideLength = context::_pixelSize * pow(SIDE_BASE, context::_sideExponent);
	_gaussPainter[VIEW]->setBaseVars(MVP, sideLength, joinDepth);
	
	_pc[GAUSS_VIEW] = new PainterCommander(_gaussPainter[VIEW], POINT_INIT_STEP);

	if (_r->_hasEdges) {
		_fieldEvaluator = new FieldEvaluation(_r->_windowWidth, _r->_windowHeight);
	} else {
		_fieldEvaluator = NULL;
	}
}