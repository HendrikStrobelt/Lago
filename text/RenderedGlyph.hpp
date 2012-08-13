#ifndef RENDERED_GLYPH_H
#define RENDERED_GLYPH_H

//include freetype2
#include <ft2build.h>
#include FT_FREETYPE_H

class RenderedGlyph {

	public:
		RenderedGlyph(int glyphIndex, FT_GlyphSlot glyph);
		~RenderedGlyph( void );

		void setCoords(float* coords, float* texCoords);

		int getGlyphIndex( void );

		int getAdvanceX( void );
		int getShiftX( void );
		int getShiftY( void );

		int getLeft( void );
		int getTop( void );
		int getWidth( void );
		int getHeight( void );

		char* getAlphaValues( void );

		float* getCoords( void );
		float* getTexCoords( void );

	private:	

		float* _coords;
		float* _texCoords;

		int _glyphIndex;
		int _advanceX;
		int _left;
		int _top;
		int _width;
		int _height;

		char* _alphaValues;
};


#endif