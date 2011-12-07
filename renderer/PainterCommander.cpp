#include "PainterCommander.hpp"
#include <math.h>

PainterCommander::PainterCommander(ISplitablePainter* painter, int initStep) {
	_painter = painter;
	_renderTime = 0.0;
	_currentIndex = 0;
	_currentStep = initStep;
}

PainterCommander::~PainterCommander( void ) {

}

//public

bool PainterCommander::isDone( void ) {
	if (_painter->getElementCount() <= _currentIndex) {
		return true;
	} else {
		return false;
	}
}

GLuint PainterCommander::getWorkingTexture( void ) {
	return _painter->getWorkingTexture();
}

GLuint PainterCommander::detachResult( void ) {
	if (isDone()) {
		_painter->detachTexture();
	} else {
		return -1;
	}
}

double PainterCommander::getRenderTime( void ) {
	return _renderTime;
}

float PainterCommander::renderNextPart( void ) {
	if (!isDone()) { 
		if (_currentIndex == 0) {
			_renderTime = 0.0;
		}
		glFinish();

		double start = glfwGetTime();
			_painter->processElements(_currentIndex, _currentStep);
			glFinish();
		double end = glfwGetTime();

		_currentIndex += _currentStep;
		_renderTime += (end - start);
		double timePerElement = (_renderTime / _currentIndex);		
		_currentStep = (ceil((double)TARGET_RENDER_TIME / timePerElement) * 1.5f);
	
		if (_currentStep > _painter->getElementCount() / 2) {
			_currentStep = (int)ceil((float)_painter->getElementCount() / 2.0f);  //show always a bar
		}
		
		return (float)_currentIndex / (float)_painter->getElementCount();
	}
	return 1.0f;
}