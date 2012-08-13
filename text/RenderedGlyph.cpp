#include "RenderedGlyph.hpp"


RenderedGlyph::RenderedGlyph(int glyphIndex, FT_GlyphSlot glyph) {
	_glyphIndex = glyphIndex;

	_width = glyph->bitmap.width;
	_height = glyph->bitmap.rows;
	_advanceX = glyph->advance.x >> 6;
	_left = glyph->bitmap_left;
	_top = glyph->bitmap_top;
	
	_alphaValues = new char[_width * _height];
	_coords = new float[8];
	_texCoords = new float[8];

	memcpy(_alphaValues, glyph->bitmap.buffer, _width * _height);
}

RenderedGlyph::~RenderedGlyph( void ) {
	delete[] _alphaValues;
	delete[] _coords;
	delete[] _texCoords;
}

//setter

void RenderedGlyph::setCoords(float* coords, float* texCoords) {
	memcpy(_coords, coords, 8 * sizeof(float));
	memcpy(_texCoords, texCoords, 8 * sizeof(float));
}

//Getter

float* RenderedGlyph::getCoords( void ) {
	return _coords;
}


float* RenderedGlyph::getTexCoords( void ) {
	return _texCoords;
}

int RenderedGlyph::getGlyphIndex( void ) {
	return _glyphIndex;
}

int RenderedGlyph::getAdvanceX( void ) {
	return _advanceX;
}

int RenderedGlyph::getLeft( void ) {
	return _left;
}

int RenderedGlyph::getTop( void ) {
	return _top;
}

int RenderedGlyph::getShiftX( void ) {
	return _left;
}

int RenderedGlyph::getShiftY( void ) {
	return -(_height - _top);
}

int RenderedGlyph::getWidth( void ) {
	return _width;
}

int RenderedGlyph::getHeight( void ) {
	return _height;
}

char* RenderedGlyph::getAlphaValues( void ) {
	return _alphaValues;
}