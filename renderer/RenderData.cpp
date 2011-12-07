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

	 _gaussTex = -1;
	 _evalField = -1;
	 _lineField = -1;

	 _maxValuesN[0] = -1;
	 _maxValuesN[1] = -1;
	 _maxValuesN[2] = -1;

	 _maxValuesE[0] = -1;
	 _maxValuesE[1] = -1;
	 _maxValuesE[2] = -1;

}