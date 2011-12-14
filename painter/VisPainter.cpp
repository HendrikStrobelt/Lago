#include "VisPainter.hpp"		
#include "../context/Context.hpp"
#include "../helper/EnvironmentHelper.hpp"

GLSLShader* VisPainter::_n_shader_ptr = NULL;
GLSLShader* VisPainter::_e_shader_ptr = NULL;
GLuint VisPainter::_colorSchemesTex = -1;

VisPainter::VisPainter(int width, int height ) {
	_width = width;
	_height = height;
	_done = false;
	_fbcRes = new FrameBufferContainer(width, height);
	_fbcInter = new FrameBufferContainer(width, height);
	
	loadTexturesOnce();
	createShader();
	initVao();
}


VisPainter::~VisPainter( void) {
	delete _fbcRes;
	delete _fbcInter;
	glDeleteBuffers(2, &_vbo[0]);
	glDeleteVertexArrays(1, &_vao);
}

void VisPainter::cleanUp( void ) {
	glDeleteTextures(1, &_colorSchemesTex);
	delete _n_shader_ptr;
	delete _e_shader_ptr;
}


void VisPainter::loadTexturesOnce( void ) {
	if (_colorSchemesTex == -1) {
		_colorSchemesTex = envHelper::loadRGBTexture("_Tex\\bluebrown.tga");
	}
}

//public

bool VisPainter::isDone( void ) {
	return _done;
}

GLuint VisPainter::detachResult( void ) {
	return _fbcRes->detachTexture();
}

void VisPainter::renderVis(RenderData* rData, bool withEdges) {
	if (!withEdges) {
		glBindFramebuffer(GL_FRAMEBUFFER, _fbcRes->_fbo);
			renderNodes(rData);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	} else {
		glBindFramebuffer(GL_FRAMEBUFFER, _fbcInter->_fbo);	
			renderNodes(rData);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, _fbcRes->_fbo);
			renderEdges(rData, _fbcInter->_fboOutTex);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	_done = true;
}

//privat

void VisPainter::renderNodes(RenderData* rData)  {
	glBlendFunc(GL_ONE, GL_ZERO);

	int scaleMode = 0;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, rData->_gaussTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, _colorSchemesTex);
			glBindVertexArray(_vao);
				_n_shader_ptr->use();			
					glUniform1i(_n_shader_ptr->getUniformLocation("gaussTex"), 0);
					glUniform1i(_n_shader_ptr->getUniformLocation("colorSchemesTex"), 1);
					glUniform1f(_n_shader_ptr->getUniformLocation("maxValue"), rData->_maxValuesN[2]);
					glUniform1i(_n_shader_ptr->getUniformLocation("antiAlias"), context::_options._antiAlias);
					glUniform1i(_n_shader_ptr->getUniformLocation("width"), _width);
					glUniform1i(_n_shader_ptr->getUniformLocation("height"), _height);
					float* cp = &(context::_scaleOptions[scaleMode]._controlPoints[0][0]);
					glUniform1i(_n_shader_ptr->getUniformLocation("linearMode"), context::_scaleOptions[scaleMode]._linearMode);
					glUniform4f(_n_shader_ptr->getUniformLocation("pointsX"), cp[0], cp[2], cp[4], cp[6]);
					glUniform4f(_n_shader_ptr->getUniformLocation("pointsY"), cp[1], cp[3], cp[5], cp[7]);
					glUniform1f(_n_shader_ptr->getUniformLocation("exponent"), context::_scaleOptions[scaleMode]._exponent);
					//draw a textured quad over the whole screen
					glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); 
				_n_shader_ptr->unUse();
			glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,  0);
}

void VisPainter::renderEdges(RenderData* rData, GLuint nodeTex) {
	glBlendFunc(GL_ONE, GL_ZERO);

	int scaleMode = 1;	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, rData->_lineField);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, nodeTex);

			glBindVertexArray(_vao);
				_e_shader_ptr->use();			
					glUniform1i(_e_shader_ptr->getUniformLocation("gaussTex"), 1);
					glUniform1i(_e_shader_ptr->getUniformLocation("lineField"), 0);
					glUniform1f(_e_shader_ptr->getUniformLocation("maxValue"), rData->_maxValuesE[1]);
					glUniform1i(_e_shader_ptr->getUniformLocation("antiAlias"), context::_options._antiAlias);
					glUniform1i(_e_shader_ptr->getUniformLocation("width"), _width);
					glUniform1i(_e_shader_ptr->getUniformLocation("height"), _height);
					float* cp = &(context::_scaleOptions[scaleMode]._controlPoints[0][0]);
					glUniform1i(_e_shader_ptr->getUniformLocation("linearMode"), context::_scaleOptions[scaleMode]._linearMode);
					glUniform4f(_e_shader_ptr->getUniformLocation("pointsX"), cp[0], cp[2], cp[4], cp[6]);
					glUniform4f(_e_shader_ptr->getUniformLocation("pointsY"), cp[1], cp[3], cp[5], cp[7]);
					glUniform1f(_e_shader_ptr->getUniformLocation("exponent"), context::_scaleOptions[scaleMode]._exponent);
					glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); 
				_e_shader_ptr->unUse();
			glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void VisPainter::createShader( void ) {
	if (_n_shader_ptr == NULL) {
		vector<string> unis;
		vector<string> attribs;

		attribs.push_back("vTex");
		attribs.push_back("vVertex");
		unis.push_back("maxValue");
		unis.push_back("antiAlias");
		unis.push_back("gaussTex");
		unis.push_back("colorSchemesTex");
		
		unis.push_back("height");
		unis.push_back("width");

		unis.push_back("linearMode");
		unis.push_back("pointsX");
		unis.push_back("pointsY");
		unis.push_back("exponent");

		_n_shader_ptr = new GLSLShader(attribs, unis, "shaders/vis/visShaderNode.vert", "shaders/vis/visShaderNode.frag");

		unis.push_back("lineField");
		_e_shader_ptr = new GLSLShader(attribs, unis, "shaders/vis/visShaderEdge.vert", "shaders/vis/visShaderEdge.frag");
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
			glEnableVertexAttribArray(_n_shader_ptr->getAttributeLocation("vVertex"));
			glVertexAttribPointer (_n_shader_ptr->getAttributeLocation("vVertex"), 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(_e_shader_ptr->getAttributeLocation("vVertex"));
			glVertexAttribPointer (_e_shader_ptr->getAttributeLocation("vVertex"), 2, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer (GL_ARRAY_BUFFER, _vbo[TEX_COORDS]);
			glEnableVertexAttribArray(_n_shader_ptr->getAttributeLocation("vTex"));
			glVertexAttribPointer (_n_shader_ptr->getAttributeLocation("vTex"), 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(_e_shader_ptr->getAttributeLocation("vTex"));
			glVertexAttribPointer (_e_shader_ptr->getAttributeLocation("vTex"), 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);
}