#ifndef GAUSS_PAINTER_H
#define GAUSS_PAINTER_H

#include <vector>
#include <glm\glm.hpp>

#include "../helper/GLSLShader.hpp"
#include "ISplitablePainter.hpp"
#include "../helper/FrameBufferContainer.hpp"

#define EX_QUAD_SIDE 6

class GaussPainter : public ISplitablePainter {

	public:
		GaussPainter(GLuint nodeVBO, int width, int height, int elementCount);
		~GaussPainter( void );
	
		void setBaseVars(glm::mat4 MVP, float quadSideLength, int pixelQuad, int nodeDepth);
		void preRenderGauss();

		//interface methods
		int getElementCount( void );
		void processElements(int start, int count);

		GLuint getWorkingTexture( void );
		GLuint detachTexture( void );

		GLuint detachSeedTexture( void );
		GLuint getSeedTexture( void );

		//static clean up
		static void cleanUp( void );

	private:
		static GLSLShader* _shader_ptr;
		static GLSLShader* _point_shader_ptr;
		static GLuint _gaussTex;
		static void createShader( void );
		static void loadTexturesOnce( void );
		
		void renderGauss(int start, int count);
		void initVao(GLuint vbo);
		void initPointVao( void );
		int _elementCount;

		int _width;
		int _height;
		GLuint _pointTexture;

		FrameBufferContainer* _fbc;
		FrameBufferContainer* _fbcPoint;

		static int _offWidth;
		static int _offHeight;
		static GLuint _pointVao;
		static GLuint _pointVbo;
				
		glm::mat4 _MVP;
		float _quadSideLength;
		int _nodeDepth;

		static int _exQuads;

		GLuint _vao;
};

#endif