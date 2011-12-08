#include "WorkStateHelper.hpp"

#include "../../helper/CameraHelper.hpp"
#include "../../context/Context.hpp"
#include "../../GlobalConstants.hpp"

#include <glm/gtc/matrix_transform.hpp>

WorkStateHelper::WorkStateHelper(Renderer* renderer) {
	_r = renderer;

	_gaussPainter[VIEW] = NULL;
	_gaussPainter[OFF] = NULL;
	_linePainter = NULL;

	_pc[GAUSS_VIEW] = NULL;
	_pc[GAUSS_OFF] = NULL;
	_pc[DIVIDED_LINES] = NULL;
	
	_fieldEvaluator = NULL;
	_progress = 0.0f;
}	

WorkStateHelper::~WorkStateHelper( void ) {
	resetAll();
}


bool WorkStateHelper::isDone( void ) {
	if (_r->_hasEdges) {
		return (_pc[GAUSS_VIEW]->isDone() &&
			    _fieldEvaluator->isDone() &&
				_pc[DIVIDED_LINES]->isDone());
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
		} else 
		if (!_pc[DIVIDED_LINES]->isDone()) {
			_progress = _pc[DIVIDED_LINES]->renderNextPart();
		} else {
			_progress = 1.0f;
		}
	} else {
		_progress = _pc[GAUSS_VIEW]->renderNextPart();
	}
}

void WorkStateHelper::takeOver( void ) {
	
	resetAll();

	int joinDepth = _r->dCache.getNodeStructureInfo()->getJoinDepth(context::_pixelSize);
	int nodeElements = _r->dCache.getNodeStructureInfo()->getAllNodes(joinDepth);
	float sideLength = context::_pixelSize * pow(SIDE_BASE, context::_sideExponent);

	glm::mat4 P = cameraHelper::calculateProjection(_r->dCache.getNodeStructureInfo(), context::_zoomFactor);
	glm::mat4 MVP = glm::translate(P, glm::vec3(context::_worldTransX, context::_worldTransY, 0.0f));

	_gaussPainter[VIEW] = new GaussPainter(_r->_nodeVBO, _r->_windowWidth, _r->_windowHeight, nodeElements);
	_gaussPainter[VIEW]->setBaseVars(MVP, sideLength, joinDepth);
	
	_pc[GAUSS_VIEW] = new PainterCommander(_gaussPainter[VIEW], POINT_INIT_STEP);

	if (_r->_hasEdges) {
		int edgeElements = _r->dCache.getEdgeStructureInfo()->getAllEdges(joinDepth);
				
		_fieldEvaluator = new FieldEvaluation(_r->_windowWidth, _r->_windowHeight);
		_linePainter = new DividedLinePainter(_r->_edgeVBO, _r->_windowWidth, _r->_windowHeight, edgeElements);
		_linePainter->setBaseVars(MVP, _fieldEvaluator->getWorkingTexture(), _fieldEvaluator->getWorkingTexture(), 1.0f, joinDepth);

		_pc[DIVIDED_LINES] = new PainterCommander(_linePainter, PARTS_INIT_STEP);
	}
}


//private

void WorkStateHelper::resetAll( void ) {
	delete _gaussPainter[VIEW];
	delete _gaussPainter[OFF];
	delete _linePainter;

	delete _pc[GAUSS_VIEW];
	delete _pc[GAUSS_OFF];
	delete _pc[DIVIDED_LINES];

	delete _fieldEvaluator;

	_gaussPainter[VIEW] = NULL;
	_gaussPainter[OFF] = NULL;
	_linePainter = NULL;

	_pc[GAUSS_VIEW] = NULL;
	_pc[GAUSS_OFF] = NULL;
	_pc[DIVIDED_LINES] = NULL;
	
	_fieldEvaluator = NULL;
	_progress = 0.0f;
}