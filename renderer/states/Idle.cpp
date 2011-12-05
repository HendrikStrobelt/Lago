#include "Idle.hpp"
#include "../Renderer.hpp"

Idle::Idle(Renderer* renderer) {
	_renderer = renderer;
}

Idle::~Idle( void ) {

}

void Idle::render( void ) {};
void Idle::renderGauss( void ) {};
void Idle::renderEvalField( void ) {};
void Idle::work( void ) {};
void Idle::changePanning( void ) {};
void Idle::changeZoom( void ) {};
void Idle::changeData( void ) {};
void Idle::changeAspectRatio( void ) {};