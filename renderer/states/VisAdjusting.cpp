#include "VisAdjusting.hpp"
#include "../Renderer.hpp"
#include "../../context/Context.hpp"
#include "../Cinema.hpp"

VisAdjusting::VisAdjusting(Renderer* renderer) {
	_r = renderer;
	_visPainter = NULL;
	_workBlendData = NULL;
}

VisAdjusting::~VisAdjusting( void ) {
	delete _visPainter;
	delete _workBlendData;
}


//Interface methods

void VisAdjusting::render( void ) {

	float maxVals[2];
	maxVals[0] = _workRData->getNodeMax();
	maxVals[1] = _workRData->getEdgeMax();

	_r->renderGraph(_workRData);
	_r->renderHUD(_workProcess, _workBars, maxVals);
	_r->renderLabelSelection(_r->_newData);

}

void VisAdjusting::renderGauss( void ) { /* nothing to do */ }
void VisAdjusting::renderEvalField( void ) { /* nothing to do */ }
void VisAdjusting::renderLineField( void ) { /* nothing to do */ }

void VisAdjusting::work( void ) {
	if (context::_options._animation && _process < 1.0f) {

		if (Cinema::_captureTime != -1.0) {
			_process = (Cinema::_captureTime - _animationStart) / context::_options._aniDuration;
		} else {
			_process = (context::getTime() - _animationStart) / context::_options._aniDuration;
		}

		delete _workBlendData;
		_workBlendData = new RenderBlendData(_r->_newData);		
		_workProcess = -1.0f;
		_workBars = 0;
	
		if (context::_options._animation && _process < 1.0f) {
			float max[3];

			mixMaxVals(max, _r->_currentData->getNodeMaxAll(), _r->_newData->getNodeMaxAll(), _process);
			_workBlendData->setNodeMax(max);
			mixMaxVals(max, _r->_currentData->getEdgeMaxAll(), _r->_newData->getEdgeMaxAll(), _process);
			_workBlendData->setEdgeMax(max);
		
			_visPainter->renderVis(_workBlendData, _r->_hasEdges, true, _r->_currentData, _r->getStandardMVP(), _process);
			_workBlendData->setVis(_visPainter->detachResult());
			_workProcess = _process;
			_workBars = 1;

			_workRData = _workBlendData;
		} else {
			_workRData = _r->_newData;
		}
	} else {
		//done
		swap();
		delete _workBlendData;
		_workBlendData = NULL;
		_r->setState(_r->_idle);
	}
}

void VisAdjusting::takeOver( void ) {
	_r->_mouseMoveX = 0;
	_r->_mouseMoveY = 0;
	_process = 0.0f;

	if (_r->_currentData->getVis() == -1) {
		//current data is not valid dataset change etc => no animation
		_process = 1.0f;
	}

	//create valid set data in slot newData
	if (_r->_newData->getGaussTex() == -1) {
		//copy valid data set to new data
		_r->_newData->clear();
		RenderData* cleared = _r->_newData;
		_r->_newData = _r->_currentData;
		_r->_currentData = cleared;
		
		//make a weak copy of the valid data in current data
		_r->_newData->weakCopyData(_r->_currentData);
	}

	float tmp[3];
	_r->calculateMaxValues(tmp, _r->_newData->getGaussTex(), _r->_windowWidth, _r->_windowHeight);
	if (context::_options._lock) {
		context::_options._overLock = (((tmp[2] - context::_options._nodeMax) / context::_options._nodeMax) > 0.01f);
		tmp[2] = context::_options._nodeMax;
	}
	context::_options._nodeMax = tmp[2];
	_r->_newData->setNodeMax(tmp);

	if (_r->_hasEdges) {
		_r->calculateMaxValues(tmp, _r->_newData->getLineField(), _r->_windowWidth, _r->_windowHeight);
		if (context::_options._lock) {
			context::_options._overLock = (context::_options._overLock || (((tmp[1] - context::_options._edgeMax) / context::_options._edgeMax) > 0.01f));
			tmp[1] = context::_options._edgeMax;
		}
		context::_options._edgeMax = tmp[1];
		_r->_newData->setEdgeMax(tmp);
	}

	context::_options._changedLocal = true; //a bit brutal

	delete _visPainter;
	_visPainter = new VisPainter(_r->_windowWidth, _r->_windowHeight);
	_visPainter->renderVis(_r->_newData, _r->_hasEdges);
	_r->_newData->setVis(_visPainter->detachResult());

	float maxChange = max((abs(_r->_currentData->getEdgeMax() - _r->_newData->getEdgeMax()) / _r->_newData->getEdgeMax()), 
					      (abs(_r->_currentData->getNodeMax() - _r->_newData->getNodeMax()) / _r->_newData->getNodeMax()));

	if (maxChange < 0.01f && _r->_newData->getSideLength() == _r->_currentData->getSideLength()) {
		//if it is only a small change & the side length is the same no animation is needed
		_process = 1.0f;
	}

	if (_process < 1.0f) {
		_workRData = _r->_currentData;
	} else {
		_workRData = _r->_newData;
	}

	if (Cinema::_captureTime != -1.0) {
		_animationStart = Cinema::_captureTime;	
	} else {
		_animationStart = context::getTime();
	}
}

void VisAdjusting::changePanning(int xMouseMove, int yMouseMove) {
	_r->_mouseMoveX = xMouseMove;
	_r->_mouseMoveY = yMouseMove;
	swap();
	_r->setState(_r->_working);
}

void VisAdjusting::changeZoom( void ) {
	swap();
	_r->setState(_r->_working);
}

void VisAdjusting::changeData(string nodeFile, string edgeFile) {
	//double clear no swap necessary
	_r->setState(_r->_initalWork);
}

void VisAdjusting::changeSideLength( void ) {
	swap();
	_r->setState(_r->_working);
}

void VisAdjusting::changeWindow( void ) {
	//double clear no swap necessary
	_r->setState(_r->_initalWork);
}

void VisAdjusting::changeVisParameter( void ) {
	takeOver();
}


//public
void VisAdjusting::cancelAnimation( void ) {
	_process = 1.0f;
}

//private stuff
void VisAdjusting::swap( void ) {
   	_r->_currentData->clear();
   	RenderData* swap = _r->_currentData;
   	_r->_currentData = _r->_newData;
   	_r->_newData = swap;
}


void VisAdjusting::mixMaxVals(float ret[], float curVals[], float newVals[], float process) {
	for (int i = 0; i < 3; i++) {
		ret[i] = curVals[i] * (1.0f - process) + newVals[i] * process;
	}
}