#include "VisAdjusting.hpp"
#include "../Renderer.hpp"
#include "../RenderBlendData.hpp"
#include "../../context/Context.hpp"

VisAdjusting::VisAdjusting(Renderer* renderer) {
	_r = renderer;
	_visPainter = NULL;
}

VisAdjusting::~VisAdjusting( void ) {
	delete _visPainter;
}


//Interface methods

void VisAdjusting::render( void ) {
	IRenderData* rData;
	RenderBlendData blendData(_r->_newData);		
	float process = -1.0f;
	int bars = 0;

	if (context::_options._animation && _process < 1.0f) {
		float max[3];

		mixMaxVals(max, _r->_currentData->getNodeMaxAll(), _r->_newData->getNodeMaxAll(), _process);
		blendData.setNodeMax(max);
		mixMaxVals(max, _r->_currentData->getEdgeMaxAll(), _r->_newData->getEdgeMaxAll(), _process);
		blendData.setEdgeMax(max);
		
		_visPainter->renderVis(&blendData, _r->_hasEdges, true, _r->_currentData, _r->getStandardMVP(), _process);
		blendData.setVis(_visPainter->detachResult());
		process = _process;
		bars = 1;

		rData = &blendData;
	} else {
		rData = _r->_newData;
	}

	float maxVals[2];
	maxVals[0] = rData->getNodeMax();
	maxVals[1] = rData->getEdgeMax();

	_r->renderGraph(rData);
	_r->renderHUD(process, bars, maxVals);
	_r->renderLabelSelection(_r->_newData);
}

void VisAdjusting::renderGauss( void ) { /* nothing to do */ }
void VisAdjusting::renderEvalField( void ) { /* nothing to do */ }
void VisAdjusting::renderLineField( void ) { /* nothing to do */ }

void VisAdjusting::work( void ) {
	if (context::_options._animation && _process < 1.0f) {
		_process = (context::getTime() - _animationStart) / context::_options._aniDuration;
	} else {
		//done
		swap();
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

	_animationStart = context::getTime();
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