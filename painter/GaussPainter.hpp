#ifndef GAUSS_PAINTER_H
#define GAUSS_PAINTER_H

#include <vector>
#include <glm\glm.hpp>

#include "../helper/GLSLShader.hpp"
#include "ISplitablePainter.hpp"
#include "../helper/FrameBufferContainer.hpp"

class GaussPainter : public ISplitablePainter {

	public:
		GaussPainter(GLuint nodeVBO, int width, int height, int elementCount);
		~GaussPainter( void );
	
		void setBaseVars(glm::mat4 MVP, float quadSideLength, int nodeDepth);

		//interface methods
		int getElementCount( void );
		void processElements(int start, int count);

		GLuint getWorkingTexture( void );
		GLuint detachTexture( void );

		//static clean up
		static void cleanUp( void );

	private:
		static GLSLShader* _shader_ptr;
		static GLuint _gaussTex;
		static void createShader( void );
		static void loadTexturesOnce( void );
		
		void renderGauss(int start, int count);
		void initVao(GLuint vbo);
		int _elementCount;

		FrameBufferContainer* _fbc;
				
		glm::mat4 _MVP;
		float _quadSideLength;
		int _nodeDepth;

		GLuint _vao;
};

#endif