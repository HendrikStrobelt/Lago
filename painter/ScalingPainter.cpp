#include "ScalingPainter.hpp"		
#include "../context/Context.hpp"

GLSLShader* ScalingPainter::_shader_ptr = NULL;

ScalingPainter::ScalingPainter( void ) {
	context::addResizeListener(this);
	createShader();
	initVao();
}


ScalingPainter::~ScalingPainter( void) {
	glDeleteBuffers(2, &_vbo[0]);
	glDeleteVertexArrays(1, &_vao);
}

//static clean up
void ScalingPainter::cleanUp( void ) {
	delete _shader_ptr;
}

void ScalingPainter::renderScaleBars(RenderData* rData) {
	glBlendFunc(GL_ONE, GL_ZERO);

	glBindVertexArray(_vao);
		_shader_ptr->use();
					glUniform1i(_shader_ptr->getUniformLocation("colorScheme"), 0);
					glUniform1i(_shader_ptr->getUniformLocation("borderRun"), true);
					glDrawArrays(GL_TRIANGLE_STRIP,  8, 4); 
					glDrawArrays(GL_TRIANGLE_STRIP, 12, 4); 
			glBindTexture(GL_TEXTURE_2D, context::_options._nodeScheme);
					glUniform1i(_shader_ptr->getUniformLocation("colorScheme"), 0);
					glUniform1i(_shader_ptr->getUniformLocation("borderRun"), false);
					glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); 
			glBindTexture(GL_TEXTURE_2D, context::_options._edgeScheme);		
					glUniform1i(_shader_ptr->getUniformLocation("borderRun"), false);
					glDrawArrays(GL_TRIANGLE_STRIP, 4, 4); 
			glBindTexture(GL_TEXTURE_2D,  0);
		_shader_ptr->unUse();
	glBindVertexArray(0);
	


}

void ScalingPainter::resizeEvent(int width, int height) {
	updateBarPositions(width, height);
}

//private stuff

void ScalingPainter::updateBarPositions(int w, int h) {
	float stepX = 2.0f / (float)w;
	float stepY = 2.0f / (float)h;

	#define left 50.0f
	#define upper 30.0f
	#define width 12.0f
	#define height 220.0f
	#define	shift 22.0f

	float textureQuad[32] =   { (-1.0f + (left)         * stepX), (1.0f - (upper + height) * stepY),
								(-1.0f + (left + width) * stepX), (1.0f - (upper + height) * stepY),
								(-1.0f + (left)         * stepX), (1.0f - (upper)          * stepY),
								(-1.0f + (left + width) * stepX), (1.0f - (upper)          * stepY),
								(-1.0f + (left + shift)         * stepX), (1.0f - (upper + height) * stepY),
								(-1.0f + (left + width + shift) * stepX), (1.0f - (upper + height) * stepY),
								(-1.0f + (left + shift)         * stepX), (1.0f - (upper)          * stepY),
								(-1.0f + (left + width + shift) * stepX), (1.0f - (upper)          * stepY),
								
	/* border stuff */			(-1.0f + (left-2)         * stepX), (1.0f - (upper+2 + height) * stepY),
								(-1.0f + (left+2 + width) * stepX), (1.0f - (upper+2 + height) * stepY),
								(-1.0f + (left-2)         * stepX), (1.0f - (upper-2)          * stepY),
								(-1.0f + (left+2 + width) * stepX), (1.0f - (upper-2)          * stepY),
								(-1.0f + (left-2 + shift)         * stepX), (1.0f - (upper+2 + height) * stepY),
								(-1.0f + (left+2 + width + shift) * stepX), (1.0f - (upper+2 + height) * stepY),
								(-1.0f + (left-2 + shift)         * stepX), (1.0f - (upper-2)          * stepY),
								(-1.0f + (left+2 + width + shift) * stepX), (1.0f - (upper-2)          * stepY)};

	glBindBuffer (GL_ARRAY_BUFFER, _vbo[VERTEX]);
		glBufferData (GL_ARRAY_BUFFER, 32 * sizeof(float), &textureQuad[0], GL_STATIC_DRAW);
	glBindBuffer (GL_ARRAY_BUFFER, 0);
}


void ScalingPainter::createShader( void ) {
	if (_shader_ptr == NULL) {
		vector<string> unis;
		vector<string> attribs;

		attribs.push_back("vTex");
		attribs.push_back("vVertex");
		unis.push_back("colorScheme");
		unis.push_back("borderRun");

		_shader_ptr = new GLSLShader(attribs, unis, "shaders/scaleShader/ScaleShader.vert", "shaders/ScaleShader/ScaleShader.frag");
	}
}


void ScalingPainter::initVao( void ) {

	float texture[32] = { 0.0f, 1.0f,     0.0f,  0.0f,      1.0f, 1.0f,     1.0f,  0.0f,
						  0.0f, 1.0f,     0.0f,  0.0f,      1.0f, 1.0f,     1.0f,  0.0f, //fill parts
						  0.0f, 1.0f,     0.0f,  0.0f,      1.0f, 1.0f,     1.0f,  0.0f,
						  0.0f, 1.0f,     0.0f,  0.0f,      1.0f, 1.0f,     1.0f,  0.0f};
	glGenVertexArrays(1, &_vao);
	glGenBuffers (2, &_vbo[0]);


	glBindBuffer(GL_ARRAY_BUFFER, _vbo[TEX_COORDS]);
		glBufferData (GL_ARRAY_BUFFER, 32 * sizeof(float), &texture[0], GL_STATIC_DRAW);
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
	updateBarPositions(w,h);
}
