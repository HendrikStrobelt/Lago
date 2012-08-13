#include "Working.hpp"
#include "../Renderer.hpp"
#include "WorkStateHelper.hpp"
#include "../../context/Context.hpp"

Working::Working(Renderer* renderer) {
	_r = renderer;
	_worker = new WorkStateHelper(_r);
}

Working::~Working( void ) {
	delete _worker;
}

void Working::render( void ) {
	_r->renderGraph(_r->_currentData, _r->_mouseMoveX, _r->_mouseMoveY);
	
	int bars = 1;
	if (_r->_hasEdges) {
		bars = 3;
	}
	float maxVals[2];
	maxVals[0] = _r->_currentData->getNodeMax();
	maxVals[1] = _r->_currentData->getEdgeMax();
	_r->renderHUD(_worker->_progress, bars, maxVals);
	_r->renderLabelSelection(_r->_currentData, _r->_mouseMoveX, _r->_mouseMoveY);
}

void Working::renderGauss( void ) { /*do nothing*/ };

void Working::renderEvalField( void ) { /*do nothing*/ };

void Working::renderLineField( void ) { /*do nothing*/ };

void Working::work( void ) {
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

		_r->_labelSelectionPainter.changePanning(_r->_mouseMoveX, _r->_mouseMoveY);
		_r->_labelPainter.updateMVP(_r->getStandardMVP());
		_r->setState(_r->_visAdjust);
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

void Working::changeVisParameter( void ) { /* do nothing */ }