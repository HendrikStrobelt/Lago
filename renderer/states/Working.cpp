#include "Working.hpp"
#include "../Renderer.hpp"

Working::Working(Renderer* renderer) {
	_renderer = renderer;
}

Working::~Working( void ) {

}

void Working::render( void ) {};
void Working::renderGauss( void ) {};
void Working::renderEvalField( void ) {};
void Working::work( void ) {};
void Working::changePanning( void ) {};
void Working::changeZoom( void ) {};
void Working::changeData( void ) {};
void Working::changeAspectRatio( void ) {};