#include "TextureExaminer.hpp"
#include "../context/Context.hpp"

GLSLShader* TextureExaminer::_shader_ptr = NULL;

TextureExaminer::TextureExaminer( void ) {
	createShader();
	initVao();
	_maxValFBO = NULL;
}

TextureExaminer::~TextureExaminer( void ) {
	glDeleteBuffers(2, &_vbo[0]);
	glDeleteVertexArrays(1, &_vao);

	if (_maxValFBO != NULL) {
		delete _maxValFBO;
	}
	
	if (_inTex != _orgTex) {
		glDeleteTextures(1, &_inTex);
	}
}

void TextureExaminer::cleanUp() {
	delete _shader_ptr;
}

void TextureExaminer::createShader( void ) {
	if (_shader_ptr == NULL) {
		vector<string> unis;
		vector<string> attribs;

		attribs.push_back("vTex");
		attribs.push_back("vVertex");
		unis.push_back("inputTexture");
		unis.push_back("kernelSize");
		unis.push_back("width");
		unis.push_back("height");
		unis.push_back("parentWidth");
		unis.push_back("parentHeight");
		_shader_ptr = new GLSLShader(attribs, unis, "shaders/maxVal/MaxValRenderer.vert", "shaders/maxVal/MaxValRenderer.frag");
	}
}




//orgTex will not be altered or deleted
void TextureExaminer::getMaxValues(float result[], GLuint orgTex, int width, int height, int kernelSize, int maxIterations) {

	//set everything back to start
	_orgTex = orgTex;
	result[0] = 0.00000001f; //don't set to 0 to avoid 0 division
	result[1] = 0.00000001f;
	result[2] = 0.00000001f;

	int currentWidth = width;
	int currentHeight = height;
	//process texture

	int i = 0;

	do {
		i++;

		int parentWidth = currentWidth;
		int parentHeight = currentHeight;


		//prepare step
		if (currentWidth > 1.0) {
			currentWidth = (int) ceil(currentWidth / (float)kernelSize);
		}
		if (currentHeight > 1.0) {
			currentHeight = (int) ceil(currentHeight / (float)kernelSize);
		}


		//execute step
		setupMaxValFBO(currentWidth, currentHeight);

		glBindFramebuffer(GL_FRAMEBUFFER, _maxValFBO->_fbo);
			glViewport(0,0, (GLsizei) currentWidth, (GLsizei) currentHeight);
			render(kernelSize, currentWidth, currentHeight, parentWidth, parentHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	} while (((currentWidth * currentHeight) > 1)  && (i < maxIterations));
	
	//read pixels back _outTex contains the result

	float* data = new float[currentWidth * currentHeight * 3];
	
	glBindFramebuffer(GL_FRAMEBUFFER, _maxValFBO->_fbo);
		glReadPixels(0, 0, (GLsizei)currentWidth,  (GLsizei)currentHeight, GL_RGB, GL_FLOAT, &data[0]); 
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	for (int i = 0; i < (currentWidth * currentHeight); i++) {
		if (data[i *3] > result[0]) {
			result[0] = data[i * 3];
		}
		if (data[i *3 + 1] > result[1]) {
			result[1] = data[i * 3 + 1];
		}
		if (data[i *3 + 2] > result[2]) {
			result[2] = data[i * 3 + 2];
		}
	}

	
	//set things back
	delete data;
	int w, h;
	context::getWindowSize(&w, &h);

	glViewport(0, 0, w, h);

	//clean up
	delete _maxValFBO;
	_maxValFBO = NULL;
	
	if (_inTex != _orgTex) {         //delete old input texture
		glDeleteTextures(1, &_inTex);//but do not delete the org texture
	}		

	_inTex = 0;
}


//render to the output texture use the input texture and determine the max value in the kernel
void TextureExaminer::render(int kernelSize, int currentWidth, int currentHeight, int parentWidth, int parentHeight) {
	glBindTexture(GL_TEXTURE_2D, _inTex);
		glBindVertexArray(_vao);
			_shader_ptr->use();			
				glUniform1i(_shader_ptr->getUniformLocation("kernelSize"), kernelSize);
				glUniform1i(_shader_ptr->getUniformLocation("width"), currentWidth);
				glUniform1i(_shader_ptr->getUniformLocation("height"), currentHeight);
				glUniform1i(_shader_ptr->getUniformLocation("parentWidth"), parentWidth);
				glUniform1i(_shader_ptr->getUniformLocation("parentHeight"), parentHeight);
				glUniform1i(_shader_ptr->getUniformLocation("inputTexture"), 0); //bound texture
				//draw a textured quad over the whole screen
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); 
			_shader_ptr->unUse();
		glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D,  0);
}



void TextureExaminer::setupMaxValFBO(int width, int height) {
	
	//clean up if necessary
	if (_maxValFBO != NULL) {//its not the first render pass			
		if (_inTex != _orgTex) {         //delete old input texture
			glDeleteTextures(1, &_inTex);//but do not delete the org texture
			_inTex = 0;
		}				
		_inTex = _maxValFBO->detachTexture();//switch handles
		
		delete _maxValFBO;
	} else {
		_inTex = _orgTex;
	}

	//in every case
	_maxValFBO = new FrameBufferContainer(width, height, GL_NEAREST);
}


void TextureExaminer::initVao( void ) {
	
	float screen[8] =   { -1.0f, 1.0f,    -1.0f, -1.0f,		 1.0f, 1.0f, 	 1.0f, -1.0f};
	float screenTex[8] = { 0.0f, 1.0f,     0.0f,  0.0f,      1.0f, 1.0f,     1.0f,  0.0f}; 

	glGenVertexArrays(1, &_vao);
	glGenBuffers (2, &_vbo[0]);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo[VERTEX]);
		glBufferData (GL_ARRAY_BUFFER, 8 * sizeof(float), &screen[0], GL_STATIC_DRAW);
	glBindBuffer (GL_ARRAY_BUFFER, _vbo[TEX_COORDS]);
		glBufferData (GL_ARRAY_BUFFER, 8 * sizeof(float), &screenTex[0], GL_STATIC_DRAW);	
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(_vao);	 
		glBindBuffer (GL_ARRAY_BUFFER, _vbo[VERTEX]);
			glEnableVertexAttribArray(_shader_ptr->getAttributeLocation("vVertex"));
			glVertexAttribPointer (_shader_ptr->getAttributeLocation("vVertex"), 2, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer (GL_ARRAY_BUFFER, _vbo[TEX_COORDS]);
			glEnableVertexAttribArray(_shader_ptr->getAttributeLocation("vTex"));
			glVertexAttribPointer (_shader_ptr->getAttributeLocation("vTex"), 2, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}