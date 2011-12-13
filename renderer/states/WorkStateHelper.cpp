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
	
	_fieldEvaluator[VIEW] = NULL;
	_fieldEvaluator[OFF] = NULL;
	_progress = 0.0f;
}	

WorkStateHelper::~WorkStateHelper( void ) {
	resetAll();
}


bool WorkStateHelper::isDone( void ) {
	if (_r->_hasEdges) {
		return (_pc[GAUSS_VIEW]->isDone() &&
				_pc[GAUSS_OFF]->isDone() &&
			    _fieldEvaluator[VIEW]->isDone() &&
				_fieldEvaluator[OFF]->isDone() &&
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
		if (!_pc[GAUSS_OFF]->isDone()) {
			glViewport(0,0, (GLsizei) (_r->_windowWidth / OFF_SHRINK), (GLsizei) (_r->_windowHeight / OFF_SHRINK));
				_progress = 1.0f + _pc[GAUSS_OFF]->renderNextPart();
			glViewport(0,0, _r->_windowWidth, (GLsizei) _r->_windowHeight);
		} else 
		if (!(_fieldEvaluator[VIEW]->isDone() && _fieldEvaluator[OFF]->isDone())) {
			//evaluate both fields
			_fieldEvaluator[VIEW]->evaluate(_pc[GAUSS_VIEW]->getWorkingTexture());
			glViewport(0,0, (GLsizei) (_r->_windowWidth / OFF_SHRINK), (GLsizei) (_r->_windowHeight / OFF_SHRINK));
				_fieldEvaluator[OFF]->evaluate(_pc[GAUSS_OFF]->getWorkingTexture());
			glViewport(0,0, _r->_windowWidth, (GLsizei) _r->_windowHeight);
		} else 
		if (!_pc[DIVIDED_LINES]->isDone()) {
			_progress = 2.0f + _pc[DIVIDED_LINES]->renderNextPart();
		}
	} else {
		_progress = _pc[GAUSS_VIEW]->renderNextPart();
	}
}

void WorkStateHelper::takeOver( void ) {
	
	resetAll();
	glm::mat4 P = cameraHelper::calculateProjection(_r->dCache.getNodeStructureInfo(), context::_zoomFactor);
	glm::mat4 MVP = glm::translate(P, glm::vec3(context::_worldTransX, context::_worldTransY, 0.0f));

	int joinDepth = _r->dCache.getNodeStructureInfo()->getJoinDepth(context::_pixelSize);
	int nodeElements = _r->dCache.getNodeStructureInfo()->getAllNodes(joinDepth);
	float sideLength = context::_pixelSize * pow(SIDE_BASE, context::_sideExponent);

	_gaussPainter[VIEW] = new GaussPainter(_r->_nodeVBO, _r->_windowWidth, _r->_windowHeight, nodeElements);
	_gaussPainter[VIEW]->setBaseVars(MVP, sideLength, joinDepth);
	
	_pc[GAUSS_VIEW] = new PainterCommander(_gaussPainter[VIEW], POINT_INIT_STEP);

	if (_r->_hasEdges) {
		int edgeElements = _r->dCache.getEdgeStructureInfo()->getAllEdges(joinDepth);
		
		//gauss off
		glm::mat4 P2 = cameraHelper::calculateProjection(_r->dCache.getNodeStructureInfo(), context::_zoomFactor / (OFF_ZOOM * OFF_SHRINK));
		glm::mat4 MVP2 = glm::translate(P2, glm::vec3(context::_worldTransX, context::_worldTransY, 0.0f));

		_gaussPainter[OFF] = new GaussPainter(_r->_nodeVBO, (_r->_windowWidth / OFF_SHRINK), (_r->_windowHeight / OFF_SHRINK), nodeElements);
		_gaussPainter[OFF]->setBaseVars(MVP2, sideLength, joinDepth);
	
		_pc[GAUSS_OFF] = new PainterCommander(_gaussPainter[OFF], POINT_INIT_STEP);

		//field eval
		_fieldEvaluator[VIEW] = new FieldEvaluation(_r->_windowWidth, _r->_windowHeight);
		_fieldEvaluator[OFF] = new FieldEvaluation((_r->_windowWidth / OFF_SHRINK), (_r->_windowHeight / OFF_SHRINK));

		//line painter
		_linePainter = new DividedLinePainter(_r->_edgeVBO, _r->_windowWidth, _r->_windowHeight, edgeElements);
		_linePainter->setBaseVars(MVP, _fieldEvaluator[VIEW]->getWorkingTexture(), _fieldEvaluator[OFF]->getWorkingTexture(), (OFF_ZOOM * OFF_SHRINK), joinDepth);

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

	delete _fieldEvaluator[VIEW];
	delete _fieldEvaluator[OFF];

	_gaussPainter[VIEW] = NULL;
	_gaussPainter[OFF] = NULL;
	_linePainter = NULL;

	_pc[GAUSS_VIEW] = NULL;
	_pc[GAUSS_OFF] = NULL;
	_pc[DIVIDED_LINES] = NULL;
	
	_fieldEvaluator[VIEW] = NULL;
	_fieldEvaluator[OFF] = NULL;
	_progress = 0.0f;
}