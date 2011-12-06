#include "Idle.hpp"
#include "../Renderer.hpp"

Idle::Idle(Renderer* renderer) {
	_r = renderer;
}

Idle::~Idle( void ) {

}

void Idle::render( void ) {
	float* maxVals = _r->_currentData->_maxValuesN;
	_r->renderTexture(_r->_currentData->_gaussTex, maxVals[0], maxVals[1], maxVals[2]);
}
void Idle::renderGauss( void ) {};
void Idle::renderEvalField( void ) {};
void Idle::work( void ) {};
void Idle::changePanning( void ) {};
void Idle::changeZoom( void ) {};
void Idle::changeData( void ) {};
void Idle::changeAspectRatio( void ) {};