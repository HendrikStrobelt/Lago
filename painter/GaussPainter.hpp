
#ifndef GAUSS_PAINTER_H
#define GAUSS_PAINTER_H

#include <vector>
#include <glm\glm.hpp>

#include "../helper/GLSLShader.hpp"
#include "ISplitablePainter.hpp"

class GaussPainter : public ISplitablePainter {

	public:
		GaussPainter(GLuint nodeVBO, int elementCount);
		~GaussPainter( void );
	
		void setBaseVars(glm::mat4 MVP, float quadSideLength, int nodeDepth);

		//interface methods
		int getElementCount( void );
		void processElements(int start, int count);

		static float getQuadSideLength(float dataWidth, float sideRatio);

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
				
		glm::mat4 _MVP;
		float _quadSideLength;
		int _nodeDepth;

		GLuint _vao;
};

#endif