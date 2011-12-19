#include "TextRenderer.hpp"
#include <glm/glm.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp> 

#include "../context/Context.hpp"

TextRenderer::TextRenderer(string fontPath, int fontSize, bool silent) {
	fillAlphabet();

	_silent = silent;
	_glyphSet = NULL;
	context::getWindowSize(&_windowWidth, &_windowHeight);
	context::addResizeListener(this);

	//init lib
	if(FT_Init_FreeType(&_ftLib)) {
		if (!_silent) {
			cerr << "Could not init freetype library" << "\n";
		}
	}

	//load face
	if(FT_New_Face(_ftLib, fontPath.c_str(), 0, &_face)) {
		if (!_silent) {
			cerr << "Could not open font " << fontPath <<  "\n";
		}
	}

	_kerning = FT_HAS_KERNING(_face);
	if (!_silent) {	
		cout << "has kerning: " << boolalpha << _kerning << "\n";
	}
	
	initShader();
	prepareBuffers();
	generateTexture();

	setFontSize(fontSize);
	_storageChange = false;
	_textLength = 0;
}

TextRenderer::~TextRenderer( void ) {
	if (_glyphSet != NULL) {
		for (int i = 0; i < _glyphSet->size(); i++) {
			delete _glyphSet->at(i);
		}
		delete _glyphSet;
	}

	FT_Done_FreeType(_ftLib);
	delete[] _ALPHABET;
	delete _shader;
	clearTextStorage();
	glDeleteBuffers(5, &_vbo[0]);
	glDeleteVertexArrays(1, &_vao);
	glDeleteTextures(1, &_fontTex); 
}


//PUBLIC

void TextRenderer::addText(string text, float centerX, float centerY, Color color) {

	float* coords1 = new float[text.length() *4];
	float* coords2 = new float[text.length() *4];
	float* texCoords1 = new float[text.length() *4];
	float* texCoords2 = new float[text.length() *4];
	float* colorArray = new float[text.length() *3];

	float colFloat[3];
	colFloat[0] = color.r;
	colFloat[1] = color.g;
	colFloat[2] = color.b;

	for (int i = 0; i < text.length(); i++) {
		memcpy(colorArray + (i*3), colFloat, 3 * sizeof(float));
	}


	//Calculate bounding box and text shifts
	int xOff = 0;
	RenderedGlyph* glyph;
	FT_UInt previousGlyph = -1;
			
	int minY =  100000000;
	int maxY = -100000000;

	float coordCopy[8];

	for (int i = 0; i < text.length(); i++) {
		int index = _ALPHABET_INDEX[text.at(i)];
		glyph = _glyphSet->at(index);
		
		int xKerning = 0;
		if(_kerning) {
			xKerning = getKerningXShift(previousGlyph, glyph->getGlyphIndex());
		}

		minY = min(glyph->getShiftY(), minY);
		maxY = max(glyph->getTop(), maxY);

		//store them
		float xShift = xOff + glyph->getLeft();
		float yShift = glyph->getShiftY();
		memcpy(coordCopy, glyph->getCoords(), 8 * sizeof(float));

		coordCopy[0] += xShift; coordCopy[2] += xShift; coordCopy[4] += xShift; coordCopy[6] += xShift;
		coordCopy[1] += yShift; coordCopy[3] += yShift; coordCopy[5] += yShift; coordCopy[7] += yShift;

		memcpy(coords1 + (i*4),    coordCopy,                 4 * sizeof(float));
		memcpy(coords2 + (i*4),    coordCopy + 4,             4 * sizeof(float));
		memcpy(texCoords1 + (i*4), glyph->getTexCoords(),     4 * sizeof(float));
		memcpy(texCoords2 + (i*4), glyph->getTexCoords() + 4, 4 * sizeof(float));
		
		//advance
		previousGlyph = glyph->getGlyphIndex();
		xOff += glyph->getAdvanceX() + xKerning;
	}


	int textHeight = maxY - minY;
	int textWidth = xOff;//could also use glyphWidth instead of advance here

	float centerShiftX = (float)textWidth / 2.0f;
	float centerShiftY = (float)textHeight / 2.0f;

	//center text
	for (int i = 0; i < text.length() * 2; i++) {
		coords1[i*2]     -= centerShiftX;
		coords1[i*2 + 1] -= centerShiftY;
		coords2[i*2]     -= centerShiftX;
		coords2[i*2 + 1] -= centerShiftY;
	}

	_storage.push_back(new PreparedText(text.size(), centerX, centerY, coords1, coords2, texCoords1, texCoords2, colorArray));
	_textLength += text.length();
	_storageChange = true;
}

void TextRenderer::clearTextStorage( void ) {
	for (int i = 0; i < _storage.size(); i++) {
		delete _storage[i];
	}
	_storage.clear();
	_textLength = 0;
	_storageChange = true;
}

void TextRenderer::resize(int windowWidth, int windowHeight) {
	_windowWidth = windowWidth;
	_windowHeight = windowHeight;
	_storageChange = true;
}


void TextRenderer::renderText(float xShift, float yShift) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//scale glyphs into standard opengl space they start at 0/0 and extend glyphWidth/glyphHeight pixels	
	glm::mat4 S = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f / (float)_windowWidth, 2.0f / (float)_windowHeight, 1.0f));

	if (_storageChange) {

		float* coords1 = new float[_textLength *4];
		float* coords2 = new float[_textLength *4];
		float* texCoords1 = new float[_textLength *4];
		float* texCoords2 = new float[_textLength *4];
		float* colorArray = new float[_textLength *3];

		int copLength = 0;
		for (int i = 0; i < _storage.size(); i++) {
			glm::vec4 shift = glm::vec4((_storage[i]->_centerX - 0.5f) * _windowWidth, (_storage[i]->_centerY - 0.5f) * _windowHeight, 0, 1);

			for (int j = 0; j < _storage[i]->_textLength * 2; j++) {
				int off = (copLength * 4) + (j*2);
				int off2 = j*2;
				coords1[off + 0] = _storage[i]->_coords1[off2 + 0] + shift.x;
				coords1[off + 1] = _storage[i]->_coords1[off2 + 1] + shift.y;
				coords2[off + 0] = _storage[i]->_coords2[off2 + 0] + shift.x;
				coords2[off + 1] = _storage[i]->_coords2[off2 + 1] + shift.y;
			}

			memcpy(texCoords1 + (copLength * 4), _storage[i]->_texCoords1, _storage[i]->_textLength * sizeof(float) * 4);
			memcpy(texCoords2 + (copLength * 4), _storage[i]->_texCoords2, _storage[i]->_textLength * sizeof(float) * 4);
			memcpy(colorArray + (copLength * 3), _storage[i]->_color, _storage[i]->_textLength * sizeof(float) * 3);
			copLength += _storage[i]->_textLength;
		}

		glBindVertexArray(_vao);	 
			glBindBuffer(GL_ARRAY_BUFFER, _vbo[VERTEX_COORD1]);
				glBufferData(GL_ARRAY_BUFFER, _textLength * 4 * sizeof(float), coords1, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, _vbo[VERTEX_COORD2]);
				glBufferData(GL_ARRAY_BUFFER, _textLength * 4 * sizeof(float), coords2, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, _vbo[TEX_COORD1]);
				glBufferData(GL_ARRAY_BUFFER, _textLength * 4 * sizeof(float), texCoords1, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, _vbo[TEX_COORD2]);
				glBufferData(GL_ARRAY_BUFFER, _textLength * 4 * sizeof(float), texCoords2, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, _vbo[COLOR]);
				glBufferData(GL_ARRAY_BUFFER, _textLength * 3 * sizeof(float), colorArray, GL_DYNAMIC_DRAW);
		glBindVertexArray(0);

		delete[] coords1;
		delete[] coords2;
		delete[] texCoords1;
		delete[] texCoords2;
		delete[] colorArray;

		_storageChange = false;
	}
	
	//render text now
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glBindTexture(GL_TEXTURE_2D, _fontTex);
			glBindVertexArray(_vao);
				_shader->use();	

					glUniform1i(_shader->getUniformLocation("tex"), 0);	
					glUniform2f(_shader->getUniformLocation("pixelShift"), xShift, yShift);
					glUniformMatrix4fv(_shader->getUniformLocation("S"), 1, GL_FALSE, glm::value_ptr(S));
					glDrawArrays(GL_POINTS, 0, _textLength);
	
				_shader->unUse();
			glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0); 
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glFlush();
}



//PRIVATE

void TextRenderer::setFontSize(int newSize) {
	FT_Set_Pixel_Sizes(_face, 0, newSize);
	int glyphW, glyphH;
	renderGlyphSet(&glyphW, &glyphH);
	uploadGlyphSet(glyphW, glyphH);
}


int TextRenderer::getKerningXShift(FT_UInt previousGlyph, FT_UInt thisGlyph) {
	if (previousGlyph == -1) {
		return 0;
	} else {
		FT_Vector delta;
		FT_Get_Kerning(_face, previousGlyph, thisGlyph, FT_KERNING_DEFAULT, &delta);
		return (delta.x >> 6);
	}
}


//uses _CHARS & _glyphSet
void TextRenderer::uploadGlyphSet(int glyphW, int glyphH) {

	int sideLength = ceil(sqrt((float)_CHARS));
	int w = sideLength * glyphW;
	int h = sideLength * glyphH;

	char* data = new char[w * h];
	memset(data, 0, w * h);
	float coords[8];
	float texCoords[8];

	//unite all glyphs into one texture
	for (int y = 0; y < sideLength; y++) {
		for (int x = 0; x < sideLength; x++) {
			int glyphIndex = y*sideLength + x; 
			if (glyphIndex < _CHARS) {
				RenderedGlyph* g = _glyphSet->at(glyphIndex);

				//write data
				for (int y2 = 0; y2 < g->getHeight(); y2++) {					
					int yOff = ((y * glyphH) + y2) * w;
					memcpy(&data[yOff + x * glyphW], &g->getAlphaValues()[y2 * g->getWidth()], g->getWidth());
				}

				//write vCoord
				coords[0] = 0.0f;
				coords[1] = 0.0f;
				coords[2] = g->getWidth();
				coords[3] = 0.0f;
				coords[4] = 0.0f;
				coords[5] = g->getHeight();
				coords[6] = g->getWidth();
				coords[7] = g->getHeight();

				//write texCoord
				texCoords[0] = (float)(x * glyphW)                  / (float) w;	//0 1
				texCoords[1] = (float)(y * glyphH + g->getHeight()) / (float) h;		
				texCoords[2] = (float)(x * glyphW + g->getWidth())  / (float) w;	//1 1
				texCoords[3] = (float)(y * glyphH + g->getHeight()) / (float) h;		
				texCoords[4] = (float)(x * glyphW)                  / (float) w;    //0 0
				texCoords[5] = (float)(y * glyphH)                  / (float) h;
				texCoords[6] = (float)(x * glyphW + g->getWidth())  / (float) w;	//1 0
				texCoords[7] = (float)(y * glyphH)                  / (float) h;

				g->setCoords(coords, texCoords);
			}
		}
	}


	//load into texture
	glBindTexture(GL_TEXTURE_2D, _fontTex);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, &data[0]);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glBindTexture(GL_TEXTURE_2D, 0);

	//clean up
	delete data;
}


//Texture and buffer preparation

void TextRenderer::initShader( void ) {
	vector<string> attribs;
	attribs.push_back("vCoord1");
	attribs.push_back("vCoord2");
	attribs.push_back("vTexCoord1");
	attribs.push_back("vTexCoord2");
	attribs.push_back("vColor");
	vector<string> unis;
	unis.push_back("pixelShift");
	unis.push_back("tex");
	unis.push_back("S");

	_shader = new GLSLShader(attribs, unis, "shaders/text/text.vert", "shaders/text/text.frag", "shaders/text/text.gem");
}


void TextRenderer::prepareBuffers( void ) {	
	glGenVertexArrays(1, &_vao);
	glGenBuffers(5, &_vbo[0]);

	glBindVertexArray(_vao);	 
		glBindBuffer(GL_ARRAY_BUFFER, _vbo[VERTEX_COORD1]);
			glEnableVertexAttribArray(_shader->getAttributeLocation("vCoord1"));
			glVertexAttribPointer (_shader->getAttributeLocation("vCoord1"), 4, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo[VERTEX_COORD2]);
			glEnableVertexAttribArray(_shader->getAttributeLocation("vCoord2"));
			glVertexAttribPointer (_shader->getAttributeLocation("vCoord2"), 4, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo[TEX_COORD1]);
			glEnableVertexAttribArray(_shader->getAttributeLocation("vTexCoord1"));
			glVertexAttribPointer (_shader->getAttributeLocation("vTexCoord1"), 4, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo[TEX_COORD2]);
			glEnableVertexAttribArray(_shader->getAttributeLocation("vTexCoord2"));
			glVertexAttribPointer (_shader->getAttributeLocation("vTexCoord2"), 4, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo[COLOR]);
			glEnableVertexAttribArray(_shader->getAttributeLocation("vColor"));
			glVertexAttribPointer (_shader->getAttributeLocation("vColor"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);
}

void TextRenderer::generateTexture( void ) {
	glGenTextures(1, &_fontTex);
	glBindTexture(GL_TEXTURE_2D, _fontTex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}


//Alphabet & Glyph Rendering

void TextRenderer::renderGlyphSet(int* maxW, int* maxH) {

	//clean up
	if (_glyphSet != NULL) {
		delete _glyphSet;
	}
	_glyphSet = new vector<RenderedGlyph*>;

	*maxW = -100;
	*maxH = -100;

	for (int i = 0; i < _CHARS; i++) {
        RenderedGlyph* g = renderGlyph(_ALPHABET[i]);
		_glyphSet->push_back(g);

		if (g->getWidth() > *maxW) {
			*maxW = g->getWidth();
		}
		if (g->getHeight() > *maxH) {
			*maxH = g->getHeight();
		}
	}
}


RenderedGlyph* TextRenderer::renderGlyph(char c) {
	int glyphIndex = FT_Get_Char_Index(_face, c);

	if(FT_Load_Glyph(_face, glyphIndex, FT_LOAD_RENDER) && !_silent) {
        cerr << "glyph rendering failed with: " << c << "\n";
	}

	return new RenderedGlyph(glyphIndex, _face->glyph);
}


void TextRenderer::fillAlphabet( void ) {
	_CHARS = 72;
	_ALPHABET = new char[_CHARS];

	int i = 0;
	_ALPHABET[i] = ' ';	_ALPHABET_INDEX[' '] = i; i++;
	_ALPHABET[i] = '(';	_ALPHABET_INDEX['('] = i; i++;
	_ALPHABET[i] = ')';	_ALPHABET_INDEX[')'] = i; i++;
	_ALPHABET[i] = '!';	_ALPHABET_INDEX['!'] = i; i++;
	_ALPHABET[i] = '.';	_ALPHABET_INDEX['.'] = i; i++;
	_ALPHABET[i] = '+';	_ALPHABET_INDEX['+'] = i; i++;
	_ALPHABET[i] = '-';	_ALPHABET_INDEX['-'] = i; i++;
	_ALPHABET[i] = ':';	_ALPHABET_INDEX[':'] = i; i++;
	_ALPHABET[i] = '_';	_ALPHABET_INDEX['_'] = i; i++;

	for (char c = '0'; c <= '9'; c++) {
		_ALPHABET[i] = c ;
		_ALPHABET_INDEX[c] = i;
		i++;
	}
	for (char c = 'A'; c <= 'Z'; c++) {
		_ALPHABET[i] = c ;
		_ALPHABET_INDEX[c] = i;
		i++;
	}
	for (char c = 'a'; c <= 'z'; c++) {
		_ALPHABET[i] = c ;
		_ALPHABET_INDEX[c] = i;
		i++;
	}
	
	if (!_silent && i != _CHARS) {
		cerr << "specified alphabet length != filled length" << "\n";
	}
}