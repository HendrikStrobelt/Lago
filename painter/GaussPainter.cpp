#include "GaussPainter.hpp"
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtc/matrix_transform.hpp> 
#include "../helper/EnvironmentHelper.hpp"
#include "../Node.hpp"


GLuint GaussPainter::_gaussTex = -1;
GLSLShader* GaussPainter::_shader_ptr = NULL;
GLSLShader* GaussPainter::_point_shader_ptr = NULL;
ExaminerGeometry GaussPainter::_exGeometry[2];


GaussPainter::GaussPainter(GLuint nodeVBO, int width, int height, int elementCount, EXAMINER_GEM fieldType) {	
	_elementCount = elementCount;
	_fbc = new FrameBufferContainer(width, height, GL_LINEAR);
	createShader();
	loadTexturesOnce();
	initVao(nodeVBO);

	_width = width;
	_height = height;
	_fieldType = fieldType;
}

GaussPainter::~GaussPainter( void ) {
	delete _fbc;
	delete _fbcPoint;
	glDeleteVertexArrays(1, &_vao);
	
	if (_pointTexture != -1) {
		glDeleteTextures(1, &_pointTexture);
	}
}

//static clean up
void  GaussPainter::cleanUp( void ) {
	delete _shader_ptr;
	delete _point_shader_ptr;
	glDeleteTextures(1, &_gaussTex);
	glDeleteVertexArrays(1, &GaussPainter::_exGeometry[OFF_FIELD]._pointVao);
	glDeleteVertexArrays(1, &GaussPainter::_exGeometry[VIEW_FIELD]._pointVao);
	glDeleteBuffers(1, &GaussPainter::_exGeometry[OFF_FIELD]._pointVbo);
	glDeleteBuffers(1, &GaussPainter::_exGeometry[VIEW_FIELD]._pointVbo);
}

//interface methods
int GaussPainter::getElementCount( void ) {
	return GaussPainter::_exGeometry[_fieldType]._exQuads;
}

void GaussPainter::processElements(int start, int count) {
	renderGauss(start, count);
}

//public

void GaussPainter::setBaseVars(glm::mat4 MVP, float quadSideLength, int pixelQuad, int nodeDepth) {
	_MVP = MVP;
	_quadSideLength = quadSideLength;
	_nodeDepth = nodeDepth;

	if (GaussPainter::_exGeometry[_fieldType]._offWidth != _width + 2.0 * pixelQuad ||
		GaussPainter::_exGeometry[_fieldType]._offHeight != _height + 2.0 * pixelQuad) {
		GaussPainter::_exGeometry[_fieldType]._offWidth = _width + 2.0 * pixelQuad;
		GaussPainter::_exGeometry[_fieldType]._offHeight =  _height + 2.0 * pixelQuad;
		initPointVao();
	}

	_fbcPoint = new FrameBufferContainer(GaussPainter::_exGeometry[_fieldType]._offWidth, GaussPainter::_exGeometry[_fieldType]._offHeight);//nearest
}

GLuint GaussPainter::getWorkingTexture( void ) {
	return _fbc->_fboOutTex;
}	

GLuint GaussPainter::detachTexture( void ) {
	return _fbc->detachTexture();
}

GLuint GaussPainter::getSeedTexture( void ) {
	return _pointTexture;
}

GLuint GaussPainter::detachSeedTexture( void ) {
	GLuint ret = _pointTexture;
	_pointTexture = -1;

	return ret;
}


void GaussPainter::preRenderGauss( void ) {
	float scaleX = (float)_width / (float)GaussPainter::_exGeometry[_fieldType]._offWidth;
	float scaleY = (float)_height / (float)GaussPainter::_exGeometry[_fieldType]._offHeight;

	glm::mat4 S = glm::scale(glm::mat4(1.0f), glm::vec3(scaleX,scaleY,1));
	glm::mat4 MVP2 = S * _MVP;

	glViewport(0,0, (GLsizei) _exGeometry[_fieldType]._offWidth, (GLsizei) _exGeometry[_fieldType]._offHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, _fbcPoint->_fbo);
	glBlendFunc(GL_ONE, GL_ONE);
		glBindVertexArray(_vao);
			_point_shader_ptr->use();			
				glUniform1i(_point_shader_ptr->getUniformLocation("desiredDepth"), _nodeDepth);
				glUniformMatrix4fv(_point_shader_ptr->getUniformLocation("MVP"), 1, GL_FALSE, glm::value_ptr(MVP2));
				glDrawArrays(GL_POINTS, 0, _elementCount);
			_point_shader_ptr->unUse();
		glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0,0, (GLsizei) _width, (GLsizei) _height);

	_pointTexture = _fbcPoint->detachTexture();
}


//private and static methods

void GaussPainter::renderGauss(int start, int count) {

	ExaminerGeometry* eg = &GaussPainter::_exGeometry[_fieldType];

	glBindFramebuffer(GL_FRAMEBUFFER, _fbc->_fbo);
		glBlendFunc(GL_ONE, GL_ONE);
		if ((start + count) > getElementCount()) {
			count = getElementCount() - start;
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _gaussTex);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, _pointTexture);
				glBindVertexArray(eg->_pointVao);
					_shader_ptr->use();			
						glUniform1i(_shader_ptr->getUniformLocation("gaussTexture"), 0);
						glUniform1i(_shader_ptr->getUniformLocation("pointTexture"), 1);
						glUniform1i(_shader_ptr->getUniformLocation("width"), eg->_offWidth);
						glUniform1i(_shader_ptr->getUniformLocation("height"), eg->_offHeight);
 						glUniformMatrix4fv(_shader_ptr->getUniformLocation("MVP"), 1, GL_FALSE, glm::value_ptr(_MVP));
						glUniform1f(_shader_ptr->getUniformLocation("sideFactor"), _quadSideLength); 
						glDrawArrays(GL_POINTS, start, count);
					_shader_ptr->unUse();
				glBindVertexArray(0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
		vector<string> unis;
		unis.push_back("MVP");
		unis.push_back("sideFactor");
		unis.push_back("gaussTexture");
		unis.push_back("width");
		unis.push_back("height");
		unis.push_back("pointTexture");
	
		_shader_ptr = new GLSLShader(attribs, unis, "shaders/gaussRenderer/gaussShader.vert",
		"shaders/gaussRenderer/gaussShader.frag", "shaders/gaussRenderer/gaussShader.gem");

		attribs.clear();
		attribs.push_back("vVertex");
		attribs.push_back("vDepth");
		attribs.push_back("vWeight");
		unis.clear();
		unis.push_back("MVP");
		unis.push_back("desiredDepth");

		_point_shader_ptr = new GLSLShader(attribs, unis, "shaders/gaussRenderer/gaussPointShader.vert",
		"shaders/gaussRenderer/gaussPointShader.frag", "shaders/gaussRenderer/gaussPointShader.gem");
	}
}

void GaussPainter::initVao(GLuint vbo) {
	glGenVertexArrays(1, &_vao);

	glBindVertexArray(_vao);	 
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glEnableVertexAttribArray(_point_shader_ptr->getAttributeLocation("vVertex"));
			glVertexAttribPointer (_point_shader_ptr->getAttributeLocation("vVertex"), 2, GL_FLOAT, GL_FALSE, sizeof(PackedNode),  0);
			glEnableVertexAttribArray(_point_shader_ptr->getAttributeLocation("vDepth"));
			glVertexAttribIPointer(_point_shader_ptr->getAttributeLocation("vDepth"), 1, GL_SHORT, sizeof(PackedNode), (void*)(2 * sizeof(float)));
			glEnableVertexAttribArray(_point_shader_ptr->getAttributeLocation("vWeight"));
			glVertexAttribPointer(_point_shader_ptr->getAttributeLocation("vWeight"), 1, GL_FLOAT, GL_FALSE, sizeof(PackedNode), (void*)(2 * sizeof(float) + sizeof(short) + sizeof(int)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void GaussPainter::initPointVao() {

	ExaminerGeometry* eg = &GaussPainter::_exGeometry[_fieldType];

	glDeleteVertexArrays(1, &eg->_pointVao);
	glDeleteBuffers(1, &eg->_pointVbo);

	glGenVertexArrays(1, &eg->_pointVao);
	glGenBuffers(1, &eg->_pointVbo);


	int wQuads = ceil((float)eg->_offWidth / (float)EX_QUAD_SIDE);
	int hQuads = ceil((float)eg->_offHeight / (float)EX_QUAD_SIDE);
	eg->_exQuads = wQuads * hQuads;

	float* initPoints = new float[eg->_exQuads * 2];

	int i = 0;
	for (int y = 0; y < hQuads; y++) {
		for (int x = 0; x < wQuads; x++) {
			initPoints[i] = x * EX_QUAD_SIDE;
			initPoints[i+1] = y * EX_QUAD_SIDE;
			i+=2;
		}
	}

	glBindBuffer (GL_ARRAY_BUFFER, eg->_pointVbo);
		glBufferData (GL_ARRAY_BUFFER, (eg->_exQuads * 2 * sizeof(float)), &initPoints[0], GL_STATIC_DRAW);
	glBindBuffer (GL_ARRAY_BUFFER, 0);

	glBindVertexArray(eg->_pointVao);	 
		glBindBuffer(GL_ARRAY_BUFFER, eg->_pointVbo);
			glEnableVertexAttribArray(_shader_ptr->getAttributeLocation("vVertex"));
			glVertexAttribPointer(_shader_ptr->getAttributeLocation("vVertex"), 2, GL_FLOAT, GL_FALSE, 0,  0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	delete initPoints;
}