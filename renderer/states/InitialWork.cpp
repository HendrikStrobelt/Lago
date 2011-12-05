#include "InitialWork.hpp"
#include "../../painter/ProgressbarPainter.hpp"

InitialWork::InitialWork( void ) {

}

InitialWork::~InitialWork( void ) {

}

void InitialWork::render( void ) {
	ProgressbarPainter::renderBar(0.4f);
};
void InitialWork::renderGauss( void ) {};
void InitialWork::renderEvalField( void ) {};
void InitialWork::work( void ) {};
void InitialWork::changePanning( void ) {};
void InitialWork::changeZoom( void ) {};
void InitialWork::changeData( void ) {};
void InitialWork::changeAspectRatio( void ) {};