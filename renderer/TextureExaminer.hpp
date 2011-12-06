#ifndef TEXTURE_EXAMINER_H
#define TEXTURE_EXAMINER_H

#include <GL\glew.h>
#include "../helper/GLSLShader.hpp"
#include "../helper/FrameBufferContainer.hpp"

class TextureExaminer {

	public:
		TextureExaminer( void );
		~TextureExaminer( void );

		void getMaxValues(float result[], GLuint orgTex, int width, int height, int kernelSize, int maxIterations);

		//static clean up
		static void cleanUp( void );
	private:
		enum DATA {VERTEX, TEX_COORDS};

		static void createShader( void );
		static GLSLShader* _shader_ptr;

		FrameBufferContainer* _maxValFBO;

		void initVao( void );
		void setupMaxValFBO(int width, int height);
		void render(int kernelSize, int currentWidth, int currentHeight, int parentWidth, int parentHeight);
		
		GLuint _vao;
		GLuint _vbo[2];

		GLuint _inTex;
		GLuint _orgTex;

};

#endif