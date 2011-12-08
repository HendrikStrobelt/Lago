#include "FieldEvaluation.hpp"		

GLSLShader* FieldEvaluation::_shader_ptr = NULL;

FieldEvaluation::FieldEvaluation(int width, int height) {
	createShader();
	initVao();

	_evalFBC = new FrameBufferContainer(width, height);
	_width = width;
	_height = height;
	_done = false;
}


FieldEvaluation::~FieldEvaluation( void) {
	glDeleteBuffers(2, &_vbo[0]);
	glDeleteVertexArrays(1, &_vao);
	delete _evalFBC;
}

void FieldEvaluation::cleanUp( void ) {
	delete _shader_ptr;
}

//public

GLuint FieldEvaluation::getWorkingTexture( void ) {
	return _evalFBC->_fboOutTex;
}

GLuint FieldEvaluation::detachResultTexture( void ) {
	return _evalFBC->detachTexture();
}

bool FieldEvaluation::isDone( void ) {
	return _done;
}

void FieldEvaluation::evaluate(GLuint texHandel) {
	glBindFramebuffer(GL_FRAMEBUFFER, _evalFBC->_fbo);
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, texHandel);
			glBindVertexArray(_vao);
				_shader_ptr->use();			
					glUniform1i(_shader_ptr->getUniformLocation("srcTexture"), 0);
					glUniform1i(_shader_ptr->getUniformLocation("width"), _width);
					glUniform1i(_shader_ptr->getUniformLocation("height"), _height);
					glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); 
				_shader_ptr->unUse();
			glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D,  0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	_done = true;
}

//private

void FieldEvaluation::createShader( void ) {
	if (_shader_ptr == NULL) {
		vector<string> unis;
		vector<string> attribs;

		attribs.push_back("vVertex");
		attribs.push_back("vTex");
	
		unis.push_back("width");
		unis.push_back("height");
		unis.push_back("srcTexture");

		_shader_ptr = new GLSLShader(attribs, unis, "shaders/fieldEvaluation/FieldEvaluation.vert", "shaders/fieldEvaluation/FieldEvaluation.frag");
	}
}


void FieldEvaluation::initVao( void ) {
	float textureQuad[8] =   { -1.0f, 1.0f,    -1.0f, -1.0f,		 1.0f, 1.0f, 	 1.0f, -1.0f};
	float texture[8] = { 0.0f, 1.0f,     0.0f,  0.0f,      1.0f, 1.0f,     1.0f,  0.0f};

	glGenVertexArrays(1, &_vao);
	glGenBuffers(2, &_vbo[0]);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo[VERTEX]);
		glBufferData (GL_ARRAY_BUFFER, 8 * sizeof(float), &textureQuad[0], GL_DYNAMIC_DRAW);
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
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}