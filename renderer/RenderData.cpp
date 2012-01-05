#include "RenderData.hpp"

RenderData::RenderData( void ) {
	clear();
}

RenderData::~RenderData( void ) {
	clear();
}

void RenderData::clear( void ) {
	if (_gaussTex != -1) {
		glDeleteTextures(1, &_gaussTex);
	}
	if (_evalField != -1) {
		glDeleteTextures(1, &_evalField);
	}
	if (_lineField != -1) {
		glDeleteTextures(1, &_lineField);
	}
	if (_lineField != -1) {
		glDeleteTextures(1, &_vis);
	}

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
