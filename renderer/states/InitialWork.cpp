#include "InitialWork.hpp"
#include "../Renderer.hpp"

InitialWork::InitialWork(Renderer* renderer) {
	_renderer = renderer;
}

InitialWork::~InitialWork( void ) {

}

void InitialWork::render( void ) {
	_renderer->renderHUD(0.4f);
};
void InitialWork::renderGauss( void ) {};
void InitialWork::renderEvalField( void ) {};
void InitialWork::work( void ) {};
void InitialWork::changePanning( void ) {};
void InitialWork::changeZoom( void ) {};
void InitialWork::changeData( void ) {};
void InitialWork::changeAspectRatio( void ) {};