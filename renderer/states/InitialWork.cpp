#include "InitialWork.hpp"
#include "../Renderer.hpp"

#include <glm/gtc/matrix_transform.hpp>

InitialWork::InitialWork(Renderer* renderer) {
	_renderer = renderer;
	int elementCount = _renderer->dCache.getNodeStrucutreInfo()->getAllNodes(_renderer->dCache.getNodeStrucutreInfo()->getMaxDepth());
	_gaussPainter = new GaussPainter(_renderer->_nodeVBO, elementCount);
	_gaussPainter->setBaseVars(glm::ortho<float>(-800, 800, -50, 850, -1, 1), 12.0f, 4);

	_pc = new PainterCommander(_gaussPainter, 1500, 750, 1);
}

InitialWork::~InitialWork( void ) {
	delete _gaussPainter;
	delete _pc;
}

void InitialWork::render( void ) {
	_renderer->renderTexture(_pc->getWorkingTexture());
}
void InitialWork::renderGauss( void ) {};
void InitialWork::renderEvalField( void ) {};
void InitialWork::work( void ) {
	if (!_pc->isDone())  {
		_pc->renderNextPart();
	}
}
void InitialWork::changePanning( void ) {};
void InitialWork::changeZoom( void ) {};
void InitialWork::changeData( void ) {};
void InitialWork::changeAspectRatio( void ) {};