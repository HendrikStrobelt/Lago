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
	float process;
	int bars;

	if (ANIMATION && _process < 1.0f) {
		float max[3];

		mixMaxVals(max, _r->_currentData->getNodeMaxAll(), _r->_newData->getNodeMaxAll(), _process);
		blendData.setNodeMax(max);
		mixMaxVals(max, _r->_currentData->getEdgeMaxAll(), _r->_newData->getEdgeMaxAll(), _process);
		blendData.setEdgeMax(max);
		
		_visPainter->renderVis(&blendData, _r->_hasEdges);
		blendData.setVis(_visPainter->detachResult());
		process = _process;
		bars = 1;

		rData = &blendData;
	} else {
		process = -1.0f;
		bars = 0;
		rData = _r->_newData;
	}

	float maxVals[2];
	maxVals[0] = rData->getNodeMax();
	maxVals[1] = rData->getEdgeMax();

	_r->renderGraph(rData, 0.0f, 0.0f);
	_r->renderHUD(process, bars, maxVals);
	_r->renderLabelSelection(_r->_newData);
}

void VisAdjusting::renderGauss( void ) { /* nothing to do */ }
void VisAdjusting::renderEvalField( void ) { /* nothing to do */ }
void VisAdjusting::renderLineField( void ) { /* nothing to do */ }

void VisAdjusting::work( void ) {
	if (ANIMATION && _process < 1.0f) {
		_process = (context::getTime() - _animationStart) / ANI_DURATION;
		cout << _process << "\n";
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
		//new data is not complete steal from _currentData
		_r->_newData->setEvalField(_r->_currentData->getEvalField());
		_r->_currentData->setEvalField(-1);
		_r->_newData->setGaussTex(_r->_currentData->getGaussTex());
		_r->_currentData->setGaussTex(-1);
		_r->_newData->setLineField(_r->_currentData->getLineField());
		_r->_currentData->setLineField(-1);
	}

	float tmp[3];
	_r->calculateMaxValues(tmp, _r->_newData->getGaussTex(), _r->_windowWidth, _r->_windowHeight);
	_r->_newData->setNodeMax(tmp);
	_r->calculateMaxValues(tmp, _r->_newData->getLineField(), _r->_windowWidth, _r->_windowHeight);
	_r->_newData->setEdgeMax(tmp);

	delete _visPainter;
	_visPainter = new VisPainter(_r->_windowWidth, _r->_windowHeight);
	_visPainter->renderVis(_r->_newData, _r->_hasEdges);
	_r->_newData->setVis(_visPainter->detachResult());

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