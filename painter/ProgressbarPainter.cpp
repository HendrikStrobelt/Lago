#include "ProgressbarPainter.hpp"		
#include "../helper/EnvironmentHelper.hpp"
#include "../context/Context.hpp"

GLSLShader* ProgressbarPainter::_shader_ptr = NULL;

ProgressbarPainter::ProgressbarPainter( void ) {
	_texBar = envHelper::loadRGBTexture("_Tex/LoadingBarBack.tga");
	_texLoadedBar = envHelper::loadRGBTexture("_Tex/LoadingBarLoaded.tga");

	context::addResizeListener(this);
	createShader();
	initVao();
}


ProgressbarPainter::~ProgressbarPainter( void) {
	glDeleteBuffers(2, &_vbo[0]);
	glDeleteVertexArrays(1, &_vao);
	glDeleteTextures(1, &_texBar);
	glDeleteTextures(1, &_texLoadedBar);
}

//static clean up
void ProgressbarPainter::cleanUp( void ) {
	delete _shader_ptr;
}

void ProgressbarPainter::renderBar(float loaded, int bars) {
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int w,h;
	context::getWindowSize(&w, &h);
	float stepY = 2.0f/(float)h;

	glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _texBar);
	glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, _texLoadedBar);

	float yOff = 15.0f * stepY * (bars-1);

	for (int i = 0; i < bars; i++) {

		float loaded2 = loaded - (bars - 1 - i);

		if (loaded2 > 1.0f) {
			loaded2 = 1.0f;
		} else if (loaded2 < 0.0f) {
			loaded2 = 0.0f;
		}

		loaded2 *= 146;
		loaded2 += 2;
		loaded2 /= 150;

		glBindVertexArray(_vao);
			_shader_ptr->use();			
				glUniform1i(_shader_ptr->getUniformLocation("barTexture"), 0); //bound texture
				glUniform1i(_shader_ptr->getUniformLocation("barLoadedTexture"), 1); //bound texture
				glUniform1f(_shader_ptr->getUniformLocation("loaded"), loaded2);
				glUniform1f(_shader_ptr->getUniformLocation("yOffset"), yOff);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); 
			_shader_ptr->unUse();
		glBindVertexArray(0);

		yOff = yOff - (15.0f * stepY);
	}

	glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,  0);
}

void ProgressbarPainter::resizeEvent(int width, int height) {
	updateBarPosition(width, height);
}

//private stuff

void ProgressbarPainter::updateBarPosition(int w, int h) {
	float stepX = 2.0f / (float)w;
	float stepY = 2.0f / (float)h;

	float textureQuad[8] =   { (1.0f - 160.0f*stepX), (1.0f - 08.0f*stepY),
							   (1.0f - 160.0f*stepX), (1.0f - 18.0f*stepY),
							   (1.0f -  10.0f*stepX), (1.0f - 08.0f*stepY),
							   (1.0f -  10.0f*stepX), (1.0f - 18.0f*stepY) };

	glBindBuffer (GL_ARRAY_BUFFER, _vbo[VERTEX]);
		glBufferData (GL_ARRAY_BUFFER, 8 * sizeof(float), &textureQuad[0], GL_STATIC_DRAW);
	glBindBuffer (GL_ARRAY_BUFFER, 0);
}



void ProgressbarPainter::createShader( void ) {
	if (_shader_ptr == NULL) {
		vector<string> unis;
		vector<string> attribs;

		attribs.push_back("vTex");
		attribs.push_back("vVertex");
		unis.push_back("barTexture");
		unis.push_back("loaded");
		unis.push_back("barLoadedTexture");
		unis.push_back("yOffset");

		_shader_ptr = new GLSLShader(attribs, unis, "shaders/barShader/BarShader.vert", "shaders/barShader/BarShader.frag");
	}
}


void ProgressbarPainter::initVao( void ) {

	float texture[8] = { 0.0f, 1.0f,     0.0f,  0.0f,      1.0f, 1.0f,     1.0f,  0.0f};

	glGenVertexArrays(1, &_vao);
	glGenBuffers (2, &_vbo[0]);


	glBindBuffer(GL_ARRAY_BUFFER, _vbo[TEX_COORDS]);
		glBufferData (GL_ARRAY_BUFFER, 8 * sizeof(float), &texture[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(_vao);	 
		glBindBuffer(GL_ARRAY_BUFFER, _vbo[VERTEX]);
			glEnableVertexAttribArray(_shader_ptr->getAttributeLocation("vVertex"));
			glVertexAttribPointer (_shader_ptr->getAttributeLocation("vVertex"), 2, GL_FLOAT, GL_FALSE, 0, 0);	
		glBindBuffer(GL_ARRAY_BUFFER, _vbo[TEX_COORDS]);
			glEnableVertexAttribArray(_shader_ptr->getAttributeLocation("vTex"));
			glVertexAttribPointer (_shader_ptr->getAttributeLocation("vTex"), 2, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer (GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	int w,h;
	context::getWindowSize(&w, &h);
	updateBarPosition(w,h);
}
