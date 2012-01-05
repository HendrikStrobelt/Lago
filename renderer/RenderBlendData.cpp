#include "RenderBlendData.hpp"


RenderBlendData::RenderBlendData(RenderData* rData) {
	clear();
	_rData = rData;
}

RenderBlendData::~RenderBlendData( void ) {
	clear();
}

void RenderBlendData::clear( void ) {
	 _maxValuesN[0] = -1;
	 _maxValuesN[1] = -1;
	 _maxValuesN[2] = -1;

	 _maxValuesE[0] = -1;
	 _maxValuesE[1] = -1;
	 _maxValuesE[2] = -1;
}


GLuint RenderBlendData::getGaussTex( void ) {
	return _rData->getGaussTex();
}

GLuint RenderBlendData::getEvalField( void ) {
	return _rData->getEvalField();
}

GLuint RenderBlendData::getLineField( void ) {
	return _rData->getLineField();
}

GLuint RenderBlendData::getVis( void ) {
	return _rData->getVis();
}

float RenderBlendData::getNodeMax( void ) {
	return _maxValuesN[2];
}

float RenderBlendData::getEdgeMax( void ) {
	return _maxValuesE[1];
}




float* RenderBlendData::getNodeMaxAll( void ) {
	return _maxValuesN;
}

float* RenderBlendData::getEdgeMaxAll( void ) {
	return _maxValuesE;
}




void RenderBlendData::setNodeMax(float max[]) {
	_maxValuesN[0] = max[0];
	_maxValuesN[1] = max[1];
	_maxValuesN[2] = max[2];
}

void RenderBlendData::setEdgeMax(float max[]) {
	_maxValuesE[0] = max[0];
	_maxValuesE[1] = max[1];
	_maxValuesE[2] = max[2];
}