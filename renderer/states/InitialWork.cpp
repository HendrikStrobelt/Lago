#include "InitialWork.hpp"
#include "../Renderer.hpp"
#include "WorkStateHelper.hpp"

#include <glm\glm.hpp>

InitialWork::InitialWork(Renderer* renderer) {
	_r = renderer;
	_worker = new WorkStateHelper(_r);
}

InitialWork::~InitialWork( void ) {
	delete _worker;
}

void InitialWork::render( void ) {
	_r->renderHUD(_worker->_progress);
}

void InitialWork::renderGauss( void ) {
	
}

void InitialWork::renderEvalField( void ) {

}


void InitialWork::work( void ) {
	if (!_worker->_pc[GAUSS_VIEW]->isDone())  {
		_worker->work();
	} else {
		//done change state
		_r->_currentData->_gaussTex = _worker->_pc[GAUSS_VIEW]->detachResult();
		_r->calculateMaxValues(_r->_currentData->_maxValuesN, _r->_currentData->_gaussTex, _r->_windowWidth, _r->_windowHeight);

		_r->setState(_r->_idle);
	}
}

void InitialWork::takeOver( void ) {
	_r->_currentData->clear();
	_r->_newData->clear();
	
	_worker->takeOver();
}


void InitialWork::changePanning( void ) {

}

void InitialWork::changeZoom( void ) {

}

void InitialWork::changeData(string nodeFile, string edgeFile) {
	_r->setState(_r->_initalWork);
}


void InitialWork::changeSideRatio( void ) {
	takeOver(); //data loading unnecessary but start from the beginning
}