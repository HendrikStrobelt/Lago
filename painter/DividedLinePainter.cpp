#include "DividedLinePainter.hpp"
#include "../Edge.hpp"
#include "../GlobalConstants.hpp"

GLSLShader* DividedLinePainter::_r_shader_ptr = NULL;
GLSLShader* DividedLinePainter::_u_shader_ptr = NULL;

DividedLinePainter::DividedLinePainter(GLuint edgeVBO, int width, int height) {
	_fbc = new FrameBufferContainer(width, height);
	_sideFactor = (float)width / (float)height;

	createShader();
	initVao(edgeVBO);
}

DividedLinePainter::~DividedLinePainter( void ) {
	delete _fbc;
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

GLuint DividedLinePainter::detachResultTexture( void ) {
	return  _fbc->detachTexture();
}

int DividedLinePainter::getElementCount( void ) {
	return (45 / ANGLE_STEP); //180 / 4
}


void DividedLinePainter::processElements(int start, int count) {

	//ensure that no old results disturbe
	GLuint _oldTex = _fb[UNITE]->detachTexture();
	glDeleteTextures(1, &_oldTex);

	if (!glIsTexture(offFieldTex)) {
		offZoomFactor = -1;
	}

	for (int j = 0; j < 2; j++) {

		bool outsideEdges =  (bool) j; //0-> false   1->true

		//at the moment run with step with 3 in 4 channels => 15 runs
		for (int i = 0; i < 180; i+=12) {
			
			float lowerBorder = i;
			float stepWidth = 3;

			//render some edges
			glBindFramebuffer(GL_FRAMEBUFFER, _fb[RENDER]->_fbo);	
				glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
				glClear(GL_COLOR_BUFFER_BIT);
				glBlendFunc(GL_ONE, GL_ONE);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, fieldTex);
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, offFieldTex);
						glBindVertexArray(_vao[RENDER]);
							_shader_ptr[RENDER]->Use();	
								glUniform1f(_shader_ptr[RENDER]->getUniformLocation("sideFactor"), _sideFactor);
								glUniform1f(_shader_ptr[RENDER]->getUniformLocation("stepWidth"), stepWidth);
								glUniform1f(_shader_ptr[RENDER]->getUniformLocation("lowerBorder"), lowerBorder);
								glUniform1i(_shader_ptr[RENDER]->getUniformLocation("desiredDepth"), desiredDepth);
								glUniform1i(_shader_ptr[RENDER]->getUniformLocation("OFF_OUT"), offOut);
								glUniform1f(_shader_ptr[RENDER]->getUniformLocation("offZoomFactor"), offZoomFactor);
								glUniform1i(_shader_ptr[RENDER]->getUniformLocation("width"), width);
								glUniform1i(_shader_ptr[RENDER]->getUniformLocation("height"), height);
								glUniform1i(_shader_ptr[RENDER]->getUniformLocation("evalField"), 0);
								glUniform1i(_shader_ptr[RENDER]->getUniformLocation("offEvalField"), 1);
								glUniform1i(_shader_ptr[RENDER]->getUniformLocation("outsideEdges"), outsideEdges);
								glUniformMatrix4fv(_shader_ptr[RENDER]->getUniformLocation("MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
								glDrawArrays(GL_POINTS, 0, _totalEdges);
							_shader_ptr[RENDER]->UnUse();
						glBindVertexArray(0);
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);


			//add them up
			GLuint assembleTex = _fb[UNITE]->detachTexture(); //get the last result and prepare a new texture

			glBindFramebuffer(GL_FRAMEBUFFER, _fb[UNITE]->_fbo);	
				glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
				glClear(GL_COLOR_BUFFER_BIT);
				glBlendFunc(GL_ONE, GL_ZERO);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, assembleTex);			//0 -> assemble tex
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, _fb[RENDER]->_fboOutTex); // 1 -> angle tex

						glBindVertexArray(_vao[UNITE]);
							_shader_ptr[UNITE]->Use();	
									glUniform1i(_shader_ptr[UNITE]->getUniformLocation("assembleTex"), 0);
									glUniform1i(_shader_ptr[UNITE]->getUniformLocation("angleTex"), 1);
									glUniform1i(_shader_ptr[UNITE]->getUniformLocation("outsideEdges"), outsideEdges);
									//draw a textured quad over the whole screen
									glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); 
							_shader_ptr[UNITE]->UnUse();
						glBindVertexArray(0);

					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, 0);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0); 

			glDeleteTextures(1, &assembleTex);
		}

	}
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
		unis.push_back("OFF_OUT");
		unis.push_back("outsideEdges");

		_r_shader_ptr = new GLSLShader(attribs, unis, "shaders/lineRenderer/climbingEdges/climbingEdges.vert",
			"shaders/lineRenderer/climbingEdges/climbingEdgesAngleLimit.frag", "shaders/lineRenderer/climbingEdges/climbingEdgesAngleLimit.gem");

		unis.clear();
		attribs.clear();

		attribs.push_back("vVertex");
		attribs.push_back("vTex");
		unis.push_back("angleTex");
		unis.push_back("assembleTex");
		unis.push_back("outsideEdges");

		_u_shader_ptr = new GLSLShader(attribs, unis, "shaders/lineRenderer/fieldEvaluation/uniteEdges.vert",
			"shaders/lineRenderer/fieldEvaluation/uniteEdges.frag");
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
