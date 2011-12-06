#include "DisplayConvertPainter.hpp"		

GLSLShader* DisplayConvertPainter::_shader_ptr = NULL;

DisplayConvertPainter::DisplayConvertPainter( void ) {
	createShader();
	initVao();
}


DisplayConvertPainter::~DisplayConvertPainter( void) {
	glDeleteBuffers(2, &_vbo[0]);
	glDeleteVertexArrays(1, &_vao);
}

//static clean up
void DisplayConvertPainter::cleanUp( void ) {
	delete _shader_ptr;
}

void DisplayConvertPainter::renderTexture(GLuint texture, float rMax, float gMax, float bMax) {
	glBlendFunc(GL_ONE, GL_ZERO);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(_vao);
		_shader_ptr->use();			
			glUniform1i(_shader_ptr->getUniformLocation("dispTexture"), 0);
			glUniform3f(_shader_ptr->getUniformLocation("maxValues"), rMax, gMax, bMax);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); 
		_shader_ptr->unUse();
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D,  0);
}

//private stuff


void DisplayConvertPainter::createShader( void ) {
	if (_shader_ptr == NULL) {
		vector<string> unis;
		vector<string> attribs;

		attribs.push_back("vTex");
		attribs.push_back("vVertex");
		unis.push_back("dispTexture");
		unis.push_back("maxValues");

		_shader_ptr = new GLSLShader(attribs, unis, "shaders/displayConvert/ConvertShader.vert", "shaders/displayConvert/ConvertShader.frag");
	}
}


void DisplayConvertPainter::initVao( void ) {
	float quad[8] = { -1.0f, 1.0f,    -1.0f, -1.0f,		 1.0f, 1.0f, 	 1.0f, -1.0f};
	float texture[8] = { 0.0f, 1.0f,     0.0f,  0.0f,      1.0f, 1.0f,     1.0f,  0.0f};

	glGenVertexArrays(1, &_vao);
	glGenBuffers (2, &_vbo[0]);


	glBindBuffer(GL_ARRAY_BUFFER, _vbo[TEX_COORDS]);
		glBufferData (GL_ARRAY_BUFFER, 8 * sizeof(float), &texture[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[VERTEX]);
		glBufferData (GL_ARRAY_BUFFER, 8 * sizeof(float), &quad[0], GL_STATIC_DRAW);
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
}
