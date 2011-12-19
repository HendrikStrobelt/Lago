#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <string>
#include <vector>
#include <iostream>
#include "../helper/GLSLShader.hpp"
#include "../context/IContextListener.hpp"
#include "RenderedGlyph.hpp"
#include "PreparedText.hpp"

//include opengl
#include <GL\glew.h>
#include <GL/glfw.h>


//include freetype2
#include <ft2build.h>
#include FT_FREETYPE_H


using namespace std;

struct Color {
	float r;
	float g;
	float b;
	Color( void ) : r(0), g(0), b(0) {};
	Color(float pR, float pG, float pB) : r(pR), g(pG), b(pB) {};
};

class TextRenderer : public  IContextListener {
	
	public:
		TextRenderer(string fontPath, int fontSize, bool silent = true);
		~TextRenderer( void );

		void addText(string text, float centerX, float centerY, Color color);
		void clearTextStorage( void );

		void renderText(float xShift=0.0f, float yShift=0.0f);
		

	private:
		
		enum VBOS {VERTEX_COORD1, VERTEX_COORD2, TEX_COORD1, TEX_COORD2, COLOR};

		void resize(int windowWidth, int windowHeight);
		void setFontSize(int newSize);
		void uploadGlyphSet(int glyphW, int glyphH);
		int getKerningXShift(FT_UInt previousGlyph, FT_UInt thisGlyph);

		void renderGlyphSet(int* maxW, int* maxH);
		void fillAlphabet( void );
		RenderedGlyph* renderGlyph(char c);

		void initShader( void );
		void prepareBuffers( void );
		void generateTexture( void );

		int _CHARS;
		char* _ALPHABET;
		char _ALPHABET_INDEX[256];

		vector<RenderedGlyph* >* _glyphSet;
		vector<PreparedText*> _storage;

		GLSLShader* _shader;
		bool _silent;
		bool _kerning;
		bool _storageChange;
		int _textLength;
		
		int _windowWidth;
		int _windowHeight;

		GLuint _fontTex;
		GLuint _vbo[5];
		GLuint _vao;

		FT_Library _ftLib;
		FT_Face _face;
};

#endif