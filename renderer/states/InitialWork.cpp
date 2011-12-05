#include "InitialWork.hpp"
#include "../Renderer.hpp"
#include "../../GlobalConstants.hpp"

#include <glm/gtc/matrix_transform.hpp>

InitialWork::InitialWork(Renderer* renderer) {
	_renderer = renderer;
	int elementCount = _renderer->dCache.getNodeStrucutreInfo()->getAllNodes(_renderer->dCache.getNodeStrucutreInfo()->getMaxDepth());
	_gaussPainter = new GaussPainter(_renderer->_nodeVBO, elementCount);
	_gaussPainter->setBaseVars(glm::ortho<float>(0, 36335, -41125, 0, -1, 1), 614.0f, _renderer->dCache.getNodeStrucutreInfo()->getMaxDepth());

	_pc = new PainterCommander(_gaussPainter, 1500, 750, POINT_INIT_STEP);
}

InitialWork::~InitialWork( void ) {
	delete _gaussPainter;
	delete _pc;
}

void InitialWork::render( void ) {
	_renderer->renderTexture(_pc->getWorkingTexture(), 1.0, 1.0, 2400000.0);
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