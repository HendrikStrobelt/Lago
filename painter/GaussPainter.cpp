#include "GaussPainter.hpp"
#include <glm/gtc/type_ptr.hpp>

#include "../helper/EnvironmentHelper.hpp"
#include "../Node.hpp"

GLuint GaussPainter::_gaussTex = -1;
GLSLShader* GaussPainter::_shader_ptr = NULL;

GaussPainter::GaussPainter(GLuint nodeVBO, int elementCount) {	
	_elementCount = elementCount;
	createShader();
	loadTexturesOnce();
	initVao(nodeVBO);
}

GaussPainter::~GaussPainter( void ) {
}

//static clean up
void  GaussPainter::cleanUp( void ) {
	delete _shader_ptr;
	glDeleteTextures(1, &_gaussTex);
}


//interface methods
int GaussPainter::getElementCount( void ) {
	return _elementCount;
}

void GaussPainter::processElements(int start, int count) {
	renderGauss(start, count);
}

//public

void GaussPainter::setBaseVars(glm::mat4 MVP, float quadSideLength, int nodeDepth) {
	_MVP = MVP;
	_quadSideLength = quadSideLength;
	_nodeDepth = nodeDepth;
}

//private and static methods

void GaussPainter::renderGauss(int start, int count) {
	if ((start + count) > _elementCount) {
		count = _elementCount - start;
	}

	glBindTexture(GL_TEXTURE_2D, _gaussTex);
		glBindVertexArray(_vao);
			_shader_ptr->use();			
				  glUniform1i(_shader_ptr->getUniformLocation("desiredDepth"), _nodeDepth);
				  glUniform1i(_shader_ptr->getUniformLocation("gaussTexture"), 0);
 				  glUniformMatrix4fv(_shader_ptr->getUniformLocation("MVP"), 1, GL_FALSE, glm::value_ptr(_MVP));
				  glUniform1f(_shader_ptr->getUniformLocation("sideFactor"), _quadSideLength); 
				  glDrawArrays(GL_POINTS, start, count);
			_shader_ptr->unUse();
		glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}


void GaussPainter::loadTexturesOnce( void ) {
	if (_gaussTex == -1) {
		_gaussTex = envHelper::loadFloatGaussTextures("_Tex\\gaussTex", 10);
	}
}

void GaussPainter::createShader( void ) {
	if (_shader_ptr == NULL) {
		vector<string> attribs;
		attribs.push_back("vVertex");
		attribs.push_back("vDepth");
		attribs.push_back("vWeight");
		vector<string> unis;
		unis.push_back("MVP");
		unis.push_back("sideFactor");
		unis.push_back("gaussTexture");
		unis.push_back("desiredDepth");
	
		_shader_ptr = new GLSLShader(attribs, unis, "shaders/pointRenderer/gauss/shader.vert",
		"shaders/pointRenderer/gauss/shader.frag", "shaders/pointRenderer/gauss/shader.gem");
	}
}

void GaussPainter::initVao(GLuint vbo) {
	glGenVertexArrays(1, &_vao);

	glBindVertexArray(_vao);	 
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glEnableVertexAttribArray(_shader_ptr->getAttributeLocation("vVertex"));
			glVertexAttribPointer (_shader_ptr->getAttributeLocation("vVertex"), 2, GL_FLOAT, GL_FALSE, sizeof(PackedNode),  0);
			glEnableVertexAttribArray(_shader_ptr->getAttributeLocation("vDepth"));
			glVertexAttribPointer (_shader_ptr->getAttributeLocation("vDepth"), 1, GL_FLOAT, GL_FALSE, sizeof(PackedNode), (void*)(2 * sizeof(float)));
			glEnableVertexAttribArray(_shader_ptr->getAttributeLocation("vWeight"));
			glVertexAttribPointer (_shader_ptr->getAttributeLocation("vWeight"), 1, GL_FLOAT, GL_FALSE, sizeof(PackedNode), (void*)(2 * sizeof(float) + sizeof(short)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
