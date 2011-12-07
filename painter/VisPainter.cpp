#include "VisPainter.hpp"		
#include "../context/Context.hpp"

GLSLShader* VisPainter::_shader_ptr = NULL;

VisPainter::VisPainter( void ) {
	createShader();
	initVao();
}


VisPainter::~VisPainter( void) {
	glDeleteBuffers(2, &_vbo[0]);
	glDeleteVertexArrays(1, &_vao);
}

void VisPainter::cleanUp( void ) {
	delete _shader_ptr;
}



void VisPainter::renderVis(RenderData* rData, float moveX, float moveY) {
	glBlendFunc(GL_ONE, GL_ZERO);

	int w,h;
	context::getWindowSize(&w, &h);

	int scaleMode = 0;//node temp

	glBindTexture(GL_TEXTURE_2D, rData->_gaussTex);
		glBindVertexArray(_vao);
			_shader_ptr->use();			
				glUniform1i(_shader_ptr->getUniformLocation("gaussTex"), 0); //bound texture
				glUniform1f(_shader_ptr->getUniformLocation("maxValue"), rData->_maxValuesN[2]);
				glUniform1i(_shader_ptr->getUniformLocation("antiAlias"), true);
				glUniform1i(_shader_ptr->getUniformLocation("width"), w);
				glUniform1i(_shader_ptr->getUniformLocation("height"), h);

				glUniform2f(_shader_ptr->getUniformLocation("move"), moveX, moveY);

				float* cp = &(context::_scaleOptions[scaleMode]._controlPoints[0][0]);
				glUniform1i(_shader_ptr->getUniformLocation("linearMode"), context::_scaleOptions[scaleMode]._linearMode);
				glUniform4f(_shader_ptr->getUniformLocation("pointsX"), cp[0], cp[2], cp[4], cp[6]);
				glUniform4f(_shader_ptr->getUniformLocation("pointsY"), cp[1], cp[3], cp[5], cp[7]);
				glUniform1f(_shader_ptr->getUniformLocation("exponent"), context::_scaleOptions[scaleMode]._exponent);
				//draw a textured quad over the whole screen
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); 
			_shader_ptr->unUse();
		glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D,  0);
}


void VisPainter::createShader( void ) {
	if (_shader_ptr == NULL) {
		vector<string> unis;
		vector<string> attribs;

		attribs.push_back("vTex");
		attribs.push_back("vVertex");
		unis.push_back("gaussTex");
		unis.push_back("maxValue");
		unis.push_back("antiAlias");
		unis.push_back("height");
		unis.push_back("width");
		unis.push_back("linearMode");
		unis.push_back("pointsX");
		unis.push_back("pointsY");
		unis.push_back("exponent");
		unis.push_back("move");

		_shader_ptr = new GLSLShader(attribs, unis, "shaders/vis/visShader.vert", "shaders/vis/visShader.frag");
	}
}


void VisPainter::initVao( void ) {
	float textureQuad[8] =   { -1.0f, 1.0f,    -1.0f, -1.0f,		 1.0f, 1.0f, 	 1.0f, -1.0f};
	float texture[8] = { 0.0f, 1.0f,     0.0f,  0.0f,      1.0f, 1.0f,     1.0f,  0.0f};

	glGenVertexArrays(1, &_vao);
	glGenBuffers (2, &_vbo[0]);

	glBindBuffer (GL_ARRAY_BUFFER, _vbo[VERTEX]);
		glBufferData (GL_ARRAY_BUFFER, 8 * sizeof(float), &textureQuad[0], GL_STATIC_DRAW);
	glBindBuffer (GL_ARRAY_BUFFER, _vbo[TEX_COORDS]);
		glBufferData (GL_ARRAY_BUFFER, 8 * sizeof(float), &texture[0], GL_STATIC_DRAW);
	glBindBuffer (GL_ARRAY_BUFFER, 0);

	glBindVertexArray(_vao);	 
		glBindBuffer (GL_ARRAY_BUFFER, _vbo[VERTEX]);
			glEnableVertexAttribArray(_shader_ptr->getAttributeLocation("vVertex"));
			glVertexAttribPointer (_shader_ptr->getAttributeLocation("vVertex"), 2, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer (GL_ARRAY_BUFFER, _vbo[TEX_COORDS]);
			glEnableVertexAttribArray(_shader_ptr->getAttributeLocation("vTex"));
			glVertexAttribPointer (_shader_ptr->getAttributeLocation("vTex"), 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);
}