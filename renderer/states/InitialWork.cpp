#include "InitialWork.hpp"
#include "../Renderer.hpp"

#include <glm/gtc/matrix_transform.hpp>

InitialWork::InitialWork(Renderer* renderer) {
	_renderer = renderer;
	int elementCount = _renderer->dCache.getNodeStrucutreInfo()->getAllNodes(_renderer->dCache.getNodeStrucutreInfo()->getMaxDepth());
	_gaussPainter = new GaussPainter(_renderer->_nodeVBO, elementCount);
	_gaussPainter->setBaseVars(glm::ortho<float>(-800, 800, -50, 850, -1, 1), 12.0f, 4);

	_fbo = new FrameBufferContainer(1500, 750, GL_LINEAR);
}

InitialWork::~InitialWork( void ) {
	delete _gaussPainter;
	delete _fbo;
}

void InitialWork::render( void ) {
	_renderer->renderTexture(_fbo->_fboOutTex, 1, 1, 70000);
}
void InitialWork::renderGauss( void ) {};
void InitialWork::renderEvalField( void ) {};
void InitialWork::work( void ) {
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo->_fbo);
		_gaussPainter->processElements(0,1000000);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void InitialWork::changePanning( void ) {};
void InitialWork::changeZoom( void ) {};
void InitialWork::changeData( void ) {};
void InitialWork::changeAspectRatio( void ) {};