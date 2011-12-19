#include "PreparedText.hpp"

PreparedText::PreparedText(int textLength, float centerX, float centerY, float* coords1, float* coords2, float* texCoords1, float* texCoords2, float* weight) {
	_coords1 = coords1;
	_coords2 = coords2;
	_texCoords1 = texCoords1;
	_texCoords2 = texCoords2;
	_weight = weight;
	_textLength = textLength;
	_centerX = centerX;
	_centerY = centerY;
}
		
			 
PreparedText::~PreparedText( void ) {
	delete[] _coords1;
	delete[] _coords2;
	delete[] _texCoords1;
	delete[] _texCoords2;
	delete[] _weight;
}