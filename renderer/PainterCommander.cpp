#include "PainterCommander.hpp"
#include <math.h>

PainterCommander::PainterCommander(ISplitablePainter* painter, int width, int height, int initStep) {
	_painter = painter;
	_fbc = new FrameBufferContainer(width, height, GL_LINEAR);
	_renderTime = 0.0;
	_currentIndex = 0;
	_currentStep = initStep;
	_done = false;
}

PainterCommander::~PainterCommander( void ) {
	delete _fbc;
}

//public

bool PainterCommander::isDone( void ) {
	return _done;
}

GLuint PainterCommander::getWorkingTexture( void ) {
	return _fbc->_fboOutTex;
}

GLuint PainterCommander::detachResult( void ) {
	if (isDone()) {
		_fbc->detachTexture();
	} else {
		return -1;
	}
}

double PainterCommander::getRenderTime( void ) {
	return _renderTime;
}

float PainterCommander::renderNextPart( void ) {
	if (!isDone()) { 
		glFinish();

		double start = glfwGetTime();
		glBindFramebuffer(GL_FRAMEBUFFER, _fbc->_fbo);
			_painter->processElements(_currentIndex, _currentStep);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glFinish();
		double end = glfwGetTime();

		_currentIndex += _currentStep;
		_renderTime += (end - start);
		double timePerPoint = (_renderTime / _currentIndex);
			
	//	_currentStep = (ceil((double)TARGET_RENDER_TIME / timePerPoint) * 1.2f);
			
		if (_currentStep > _painter->getElementCount() / 2) {
			_currentStep = (int)ceil((float)_painter->getElementCount() / 2.0f);  //show always a bar
		}

		return (float)_currentIndex / (float)_painter->getElementCount();
	}
	return 1.0f;
}