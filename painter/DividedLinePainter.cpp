#include "DividedLinePainter.hpp"
#include "../Edge.hpp"
#include "../GlobalConstants.hpp"

GLSLShader* DividedLinePainter::_r_shader_ptr = NULL;
GLSLShader* DividedLinePainter::_u_shader_ptr = NULL;

DividedLinePainter::DividedLinePainter(GLuint edgeVBO, int width, int height, int elementCount) {
	_sideFactor = (float)width / (float)height;
	_width = width;
	_height = height;
	_vboElements = elementCount;

	_fbc = NULL;

	createShader();
	initVao(edgeVBO);
}

void DividedLinePainter::setBaseVars(glm::mat4 MVP, GLuint fieldTex, GLuint offFieldTex, int offZoomFactor, int edgeDepth) {
	_uniteSwitch = 0;
	_MVP = MVP;
	_fieldTex = fieldTex;
	_offFieldTex = offFieldTex;
	_offZoomFactor = offZoomFactor;
	_edgeDepth = edgeDepth;

	if (_fbc != NULL) {
		//reuse delete first 
		delete _fbc;
		glDeleteTextures(1, &_uniteTextures[(_uniteSwitch % 2)]);
		glDeleteTextures(1, &_renderTexture);
	} 
	//generate three equal cleared textures
	_fbc = new FrameBufferContainer(_width, _height);
	_uniteTextures[0] = _fbc->detachTexture();
	_renderTexture = _fbc->detachTexture();
	_uniteTextures[1] = _fbc->_fboOutTex;
}

DividedLinePainter::~DividedLinePainter( void ) {
	delete _fbc;
	glDeleteTextures(1, &_uniteTextures[(_uniteSwitch % 2)]);
	glDeleteTextures(1, &_renderTexture);
	glDeleteVertexArrays(2, &_vao[0]);
	glDeleteBuffers(2, &_vbo[0]);
}

void DividedLinePainter::cleanUp( void ) {
	delete _r_shader_ptr;
	delete _u_shader_ptr;
}

//publics

GLuint DividedLinePainter::getWorkingTexture( void ) {
	return _fbc->_fboOutTex;
}

GLuint DividedLinePainter::detachTexture( void ) {
	return  _fbc->detachTexture();
}

int DividedLinePainter::getElementCount( void ) {
	return ceil(45.0f / (float)ANGLE_STEP); //180 / 4
}


void DividedLinePainter::processElements(int start, int count) {

	if ((start + count) > getElementCount()) {
		count = getElementCount() - start;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, _fbc->_fbo);	
		for (int i = (start*4); i < ((start*4)+(count*4)); i+= 4) {

			float lowerBorder = i * ANGLE_STEP;
			float stepWidth = ANGLE_STEP;
		
			//switch textures
			_fbc->setAttachment0NoDeleteNoBind(_renderTexture);

			//render edges
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glBlendFunc(GL_ONE, GL_ONE);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, _fieldTex);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, _offFieldTex);
					glBindVertexArray(_vao[RENDER]);
						_r_shader_ptr->use();	
							glUniform1f(_r_shader_ptr->getUniformLocation("sideFactor"), _sideFactor);
							glUniform1f(_r_shader_ptr->getUniformLocation("stepWidth"), stepWidth);
							glUniform1f(_r_shader_ptr->getUniformLocation("lowerBorder"), lowerBorder);
							glUniform1i(_r_shader_ptr->getUniformLocation("desiredDepth"), _edgeDepth);
							glUniform1f(_r_shader_ptr->getUniformLocation("offZoomFactor"), _offZoomFactor);
							glUniform1i(_r_shader_ptr->getUniformLocation("width"), _width);
							glUniform1i(_r_shader_ptr->getUniformLocation("height"), _height);
							glUniform1i(_r_shader_ptr->getUniformLocation("evalField"), 0);
							glUniform1i(_r_shader_ptr->getUniformLocation("offEvalField"), 1);
							glUniformMatrix4fv(_r_shader_ptr->getUniformLocation("MVP"), 1, GL_FALSE, glm::value_ptr(_MVP));
							glDrawArrays(GL_POINTS, 0, _vboElements);
						_r_shader_ptr->unUse();
					glBindVertexArray(0);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, 0);
		
			//switch textures
			_fbc->setAttachment0NoDeleteNoBind(_uniteTextures[(_uniteSwitch % 2)]);
		
			//add them up
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glBlendFunc(GL_ONE, GL_ZERO);
	
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, _uniteTextures[((_uniteSwitch + 1) % 2)]);//0 -> assemble tex
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, _renderTexture); // 1 -> angle tex
					glBindVertexArray(_vao[UNITE]);
						_u_shader_ptr->use();	
							glUniform1i(_u_shader_ptr->getUniformLocation("assembleTex"), 0);
							glUniform1i(_u_shader_ptr->getUniformLocation("angleTex"), 1);
							glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); 
						_u_shader_ptr->unUse();
					glBindVertexArray(0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);

			_uniteSwitch++;
		}

	glBindFramebuffer(GL_FRAMEBUFFER, 0); 
}


void DividedLinePainter::createShader( void ) {
	if (_r_shader_ptr == NULL) {
		vector<string> attribs;
		attribs.push_back("vVertex1");
		attribs.push_back("vVertex2");
		attribs.push_back("vDepth");
		attribs.push_back("vWeight");
		vector<string> unis;
		unis.push_back("MVP");
		unis.push_back("evalField");
		unis.push_back("offEvalField");
		unis.push_back("offZoomFactor");
		unis.push_back("desiredDepth");
		unis.push_back("lowerBorder");
		unis.push_back("stepWidth");
		unis.push_back("sideFactor");
		unis.push_back("width");
		unis.push_back("height");

		_r_shader_ptr = new GLSLShader(attribs, unis, "shaders/climbingLines/dividedClimbing.vert",
			"shaders/climbingLines/dividedClimbing.frag", "shaders/climbingLines/dividedClimbing.gem");

		unis.clear();
		attribs.clear();

		attribs.push_back("vVertex");
		attribs.push_back("vTex");
		unis.push_back("angleTex");
		unis.push_back("assembleTex");

		_u_shader_ptr = new GLSLShader(attribs, unis, "shaders/climbingLines/uniteEdges.vert",
			"shaders/climbingLines/uniteEdges.frag");
	}
}


void DividedLinePainter::initVao(GLuint edgeVBO) {

	glGenVertexArrays(2, &_vao[0]);
	glGenBuffers(2, &_vbo[0]);

	glBindVertexArray(_vao[RENDER]);	 
		glBindBuffer(GL_ARRAY_BUFFER, edgeVBO);
				glEnableVertexAttribArray(_r_shader_ptr->getAttributeLocation("vVertex1"));
				glVertexAttribPointer (_r_shader_ptr->getAttributeLocation("vVertex1"), 2, GL_FLOAT, GL_FALSE, sizeof(PackedEdge), 0);
				glEnableVertexAttribArray(_r_shader_ptr->getAttributeLocation("vVertex2"));
				glVertexAttribPointer (_r_shader_ptr->getAttributeLocation("vVertex2"), 2, GL_FLOAT, GL_FALSE, sizeof(PackedEdge),  (void*)(2 * sizeof(float)));
				glEnableVertexAttribArray(_r_shader_ptr->getAttributeLocation("vDepth"));
				glVertexAttribIPointer (_r_shader_ptr->getAttributeLocation("vDepth"), 1, GL_SHORT, sizeof(PackedEdge),  (void*)(4 * sizeof(float)));
				glEnableVertexAttribArray(_r_shader_ptr->getAttributeLocation("vWeight"));
				glVertexAttribPointer (_r_shader_ptr->getAttributeLocation("vWeight"), 1, GL_FLOAT, GL_FALSE, sizeof(PackedEdge),  (void*)((4 * sizeof(float) + sizeof(short))));
			glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//SCREEN QUAD

	float textureQuad[8] =   { -1.0f, 1.0f,    -1.0f, -1.0f,		 1.0f, 1.0f, 	 1.0f, -1.0f};
	float texture[8] = { 0.0f, 1.0f,     0.0f,  0.0f,      1.0f, 1.0f,     1.0f,  0.0f};

	glBindVertexArray(_vao[UNITE]);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo[UNITE_VERTEX]);
			glBufferData (GL_ARRAY_BUFFER, 8 * sizeof(float), &textureQuad[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo[UNITE_TEX]);
			glBufferData (GL_ARRAY_BUFFER, 8 * sizeof(float), &texture[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glBindVertexArray(_vao[UNITE]);	 
		glBindBuffer(GL_ARRAY_BUFFER, _vbo[UNITE_VERTEX]);	
			glEnableVertexAttribArray(_u_shader_ptr->getAttributeLocation("vVertex"));
			glVertexAttribPointer (_u_shader_ptr->getAttributeLocation("vVertex"), 2, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo[UNITE_TEX]);
			glEnableVertexAttribArray(_u_shader_ptr->getAttributeLocation("vTex"));
			glVertexAttribPointer (_u_shader_ptr->getAttributeLocation("vTex"), 2, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}
