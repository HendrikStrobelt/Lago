#include "Working.hpp"
#include "../Renderer.hpp"

Working::Working(Renderer* renderer) {
	_r = renderer;
}

Working::~Working( void ) {

}

void Working::render( void ) {};
void Working::renderGauss( void ) {};
void Working::renderEvalField( void ) {};
void Working::work( void ) {};
void Working::takeOver( void ) {};
void Working::changePanning( void ) {};
void Working::changeZoom( void ) {};
void Working::changeData(string nodeFile, string edgeFile) {};
void Working::changeAspectRatio( void ) {};