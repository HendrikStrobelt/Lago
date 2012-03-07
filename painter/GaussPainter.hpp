#ifndef GAUSS_PAINTER_H
#define GAUSS_PAINTER_H

#include <vector>
#include <glm\glm.hpp>

#include "../helper/GLSLShader.hpp"
#include "ISplitablePainter.hpp"
#include "../helper/FrameBufferContainer.hpp"

#define EX_QUAD_SIDE 6

enum EXAMINER_GEM {VIEW_FIELD, OFF_FIELD};

struct ExaminerGeometry {
	int _exQuads;
	int _offWidth;
	int _offHeight;
	GLuint _pointVao;
	GLuint _pointVbo;
	ExaminerGeometry() : _exQuads(0), _offWidth(0), _offHeight(0), _pointVao(-1), _pointVbo(-1){};
};

class GaussPainter : public ISplitablePainter {

	public:
		GaussPainter(GLuint nodeVBO, int width, int height, int elementCount, EXAMINER_GEM fieldType);
		~GaussPainter( void );
	
		void setBaseVars(glm::mat4 MVP, float quadSideLength, int pixelQuad, int nodeDepth);
		void preRenderGauss();

		//interface methods
		int getElementCount( void );
		void processElements(int start, int count);

		GLuint getWorkingTexture( void );
		GLuint detachTexture( void );

		//static clean up
		static void cleanUp( void );

	private:
		static GLSLShader* _shader_ptr;
		static GLSLShader* _point_shader_ptr;
		static GLuint _gaussTex;
		static void createShader( void );
		static void loadTexturesOnce( void );
		
		static ExaminerGeometry _exGeometry[2];

		void renderGauss(int start, int count);
		void initVao(GLuint vbo);
		void initPointVao( void );
		int _elementCount;

		int _width;
		int _height;
		GLuint _pointTexture;

		FrameBufferContainer* _fbc;
		FrameBufferContainer* _fbcPoint;
				
		glm::mat4 _MVP;
		float _quadSideLength;
		int _nodeDepth;
		EXAMINER_GEM _fieldType;

		GLuint _vao;
};

#endif