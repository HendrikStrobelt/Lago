#include "ProgressbarPainter.hpp"		
#include "../helper/EnvironmentHelper.hpp"

namespace ProgressbarPainter {

//private members and vars
enum DATA {VERTEX, TEX_COORDS};

void createShader( void );
void initVao( void );

GLSLShader* _shader_ptr;
GLuint _vao;
GLuint _vbo[2];
GLuint _texLoadedBar;
GLuint _texBar;


//public stuff

void init( void ) {
	_texBar = envHelper::loadRGBTexture("_Tex/LoadingBarBack.tga");
	_texLoadedBar = envHelper::loadRGBTexture("_Tex/LoadingBarLoaded.tga");

	createShader();
	initVao();
}


void cleanUp( void) {
	glDeleteBuffers(2, &_vbo[0]);
	glDeleteVertexArrays(1, &_vao);
	glDeleteTextures(1, &_texBar);
	glDeleteTextures(1, &_texLoadedBar);
	delete _shader_ptr;
}

void renderBar(float loaded) {
	glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _texBar);
	glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, _texLoadedBar);

	loaded *= 286;
	loaded += 7;
	loaded /= 300;

	glBindVertexArray(_vao);
		_shader_ptr->use();			
			glUniform1i(_shader_ptr->getUniformLocation("barTexture"), 0); //bound texture
			glUniform1i(_shader_ptr->getUniformLocation("barLoadedTexture"), 1); //bound texture
			glUniform1f(_shader_ptr->getUniformLocation("loaded"), loaded);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); 
		_shader_ptr->unUse();
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,  0);
}

//private stuff

void createShader( void ) {
	vector<string> unis;
	vector<string> attribs;

	attribs.push_back("vTex");
	attribs.push_back("vVertex");
	unis.push_back("barTexture");
	unis.push_back("loaded");
	unis.push_back("barLoadedTexture");

	_shader_ptr = new GLSLShader(attribs, unis, "shaders/barShader/BarShader.vert", "shaders/barShader/BarShader.frag");
}


void initVao( void ) {

	float textureQuad[8] =   { 0.58f, 0.96f,    0.58f, 0.88f,		 0.98f, 0.96f, 	 0.98f, 0.88f};
	float texture[8] = { 0.0f, 1.0f,     0.0f,  0.0f,      1.0f, 1.0f,     1.0f,  0.0f};

	//allocate and assign vao and vbo objects to the handles

	glGenVertexArrays(1, &_vao);
	glGenBuffers (2, &_vbo[0]);

	//bind vertex array = screen coordinates for the display pass
	glBindVertexArray(_vao);	 
		glBindBuffer (GL_ARRAY_BUFFER, _vbo[VERTEX]);
			glBufferData (GL_ARRAY_BUFFER, 8 * sizeof(float), &textureQuad[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(_shader_ptr->getAttributeLocation("vVertex"));
			glVertexAttribPointer (_shader_ptr->getAttributeLocation("vVertex"), 2, GL_FLOAT, GL_FALSE, 0, 0);
		
		glBindBuffer (GL_ARRAY_BUFFER, _vbo[TEX_COORDS]);
			glBufferData (GL_ARRAY_BUFFER, 8 * sizeof(float), &texture[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(_shader_ptr->getAttributeLocation("vTex"));
			glVertexAttribPointer (_shader_ptr->getAttributeLocation("vTex"), 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);
}

};//namespace end