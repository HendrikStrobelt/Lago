#include "RenderData.hpp"

RenderData::RenderData( void ) {
	_foreignTextures = false;
	clear();
}

RenderData::~RenderData( void ) {
	clear();
}

void RenderData::weakCopyData(RenderData* target) {
	target->clear();
	for (int i = 0; i < 3; i++) {
		target->_maxValuesN[i] = _maxValuesN[i];
		target->_maxValuesE[i] = _maxValuesE[i];
	}

	for (int i = 0; i < 4; i++) {
		target->_box[i] = _box[i];
	}

	target->_sideLength = _sideLength;

	target->_gaussTex = _gaussTex;
	target->_evalField = _evalField;
	target->_lineField = _lineField;
	target->_vis = _vis;

	target->_foreignTextures = true;
}

void RenderData::clear( void ) {
	if (!_foreignTextures) {
		if (_gaussTex != -1) {
			glDeleteTextures(1, &_gaussTex);
		}
		if (_evalField != -1) {
			glDeleteTextures(1, &_evalField);
		}
		if (_lineField != -1) {
			glDeleteTextures(1, &_lineField);
		}
		if (_vis != -1) {
			glDeleteTextures(1, &_vis);
		}
	}
	_foreignTextures = false;

	 _gaussTex = -1;
	 _evalField = -1;
	 _lineField = -1;
	 _vis = -1;

	 _maxValuesN[0] = -1;
	 _maxValuesN[1] = -1;
	 _maxValuesN[2] = -1;

	 _maxValuesE[0] = -1;
	 _maxValuesE[1] = -1;
	 _maxValuesE[2] = -1;

	 _box[0] = -1;
	 _box[1] = -1;
	 _box[2] = -1;
	 _box[3] = -1;

	 _sideLength = -1;
}


GLuint RenderData::getGaussTex( void ) {
	return _gaussTex;
}

GLuint RenderData::getEvalField( void ) {
	return _evalField;
}

GLuint RenderData::getLineField( void ) {
	return _lineField;
}

GLuint RenderData::getVis( void ) {
	return _vis;
}
		
float RenderData::getNodeMax( void ) {
	return _maxValuesN[2];
}

float RenderData::getEdgeMax( void ) {
	return _maxValuesE[1];
}

float* RenderData::getBox( void ) {
	return _box;
}

float RenderData::getSideLength( void ) {
	return _sideLength;
}

float* RenderData::getNodeMaxAll( void ) {
	return _maxValuesN;
}

float* RenderData::getEdgeMaxAll( void ) {
	return _maxValuesE;
}




void RenderData::setGaussTex(GLuint tex) {
	_gaussTex = tex;
}
	
void RenderData::setEvalField(GLuint tex) {
	_evalField = tex;
}

void RenderData::setLineField(GLuint tex) {
	_lineField = tex;
}

void RenderData::setVis(GLuint tex) {
	_vis = tex;
}

void RenderData::setNodeMax(float max[]) {
	_maxValuesN[0] = max[0];
	_maxValuesN[1] = max[1];
	_maxValuesN[2] = max[2];
}

void RenderData::setEdgeMax(float max[]) {
	_maxValuesE[0] = max[0];
	_maxValuesE[1] = max[1];
	_maxValuesE[2] = max[2];
}

void RenderData::setBox(float box[]) {
	_box[0] = box[0];
	_box[1] = box[1];
	_box[2] = box[2];
	_box[3] = box[3];
}

void RenderData::setSideLength(float length) {
	_sideLength = length;
}