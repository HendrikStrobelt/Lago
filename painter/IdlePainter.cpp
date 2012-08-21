#include "IdlePainter.hpp"		
#include "../helper/EnvironmentHelper.hpp"
#include "../context/Context.hpp"

GLSLShader* IdlePainter::_shader_ptr = NULL;

IdlePainter::IdlePainter( void ) {
	_tex = envHelper::loadSmoothRGBTexture("_Tex/IdleLogo.tga");
	context::addResizeListener(this);

	createShader();
	initVao();
}


IdlePainter::~IdlePainter( void) {
	glDeleteBuffers(2, &_vbo[0]);
	glDeleteVertexArrays(1, &_vao);
	glDeleteTextures(1, &_tex);
}

//static clean up
void IdlePainter::cleanUp( void ) {
	delete _shader_ptr;
}

void IdlePainter::render() {
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _tex);

		glBindVertexArray(_vao);
			_shader_ptr->use();			
				glUniform1i(_shader_ptr->getUniformLocation("tex"), 0); //bound texture
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); 
			_shader_ptr->unUse();
		glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,  0);
}

//private stuff




void IdlePainter::createShader( void ) {
	if (_shader_ptr == NULL) {
		vector<string> unis;
		vector<string> attribs;

		attribs.push_back("vTex");
		attribs.push_back("vVertex");
		unis.push_back("tex");

		_shader_ptr = new GLSLShader(attribs, unis, "shaders/IdleLogo/IdleLogo.vert", "shaders/IdleLogo/IdleLogo.frag");
	}
}

void IdlePainter::resizeEvent(int width, int height) {
	float stepX = 2.0 / width;
	float stepY = 2.0 / height;

	float textureQuad[8] =   { (- 200.0f*stepX), (  200.0f*stepY),
							   (- 200.0f*stepX), (- 200.0f*stepY),
							   (  200.0f*stepX), (  200.0f*stepY),
							   (  200.0f*stepX), (- 200.0f*stepY) };

	glBindBuffer (GL_ARRAY_BUFFER, _vbo[VERTEX]);
		glBufferData (GL_ARRAY_BUFFER, 8 * sizeof(float), &textureQuad[0], GL_STATIC_DRAW);
	glBindBuffer (GL_ARRAY_BUFFER, 0);
}

void IdlePainter::initVao( void ) {

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
	resizeEvent(w,h);
}
