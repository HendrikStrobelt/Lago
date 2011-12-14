#include "RenderData.hpp"

RenderData::RenderData( void ) {
	_isShadowCopy = false;
	clear();
}

RenderData::RenderData(RenderData* par) {
	_isShadowCopy = true;
	_gaussTex = par->_gaussTex;
	_evalField = par->_evalField;
	_lineField = par->_lineField;
	_vis = par->_vis;

	_maxValuesN[0] = par->_maxValuesN[0];
	_maxValuesN[1] = par->_maxValuesN[1];
	_maxValuesN[2] = par->_maxValuesN[2];
	_maxValuesE[0] = par->_maxValuesE[0];
	_maxValuesE[1] = par->_maxValuesE[1];
	_maxValuesE[2] = par->_maxValuesE[2];
}

RenderData::~RenderData( void ) {
	if (!_isShadowCopy) {
		clear();
	}
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