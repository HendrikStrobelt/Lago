#ifndef DIVIDED_LINE_PAINTER_H
#define DIVIDED_LINE_PAINTER_H

#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include "../helper/FrameBufferContainer.hpp"
#include "../helper/GLSLShader.hpp"
#include "ISplitablePainter.hpp"

using namespace std;

class DividedLinePainter : public ISplitablePainter {

	public:
		DividedLinePainter(GLuint edgeVBO, GLuint processedEdgeVBO, int width, int height, int vboElements);
		~DividedLinePainter( void );

		void setBaseVars(glm::mat4 MVP, GLuint fieldTex, GLuint offFieldTex, int offZoomFactor, int edgeDepth);
		void preprocessElements( void );

		//interface methods
		int getElementCount( void );
		void processElements(int start, int count);

		GLuint getWorkingTexture( void );
		GLuint detachTexture( void );

		//static clean up
		static void cleanUp( void );

	private:
		enum VAO {RENDER, UNITE, PREPROCESS};
		enum VBO {UNITE_VERTEX, UNITE_TEX};

		static GLSLShader* _r_shader_ptr;
		static GLSLShader* _u_shader_ptr;
		static GLSLShader* _p_shader_ptr;
		static void createShader( void );
		
		void initVao(GLuint edgeVBO, GLuint preprocessedEdgeVBO);

		GLuint _renderTexture;
		GLuint _uniteTextures[2];
		int _uniteSwitch;

		float _sideFactor;
		int _width;
		int _height;
		int _vboElements;
		int _vboProcessedElements;

		glm::mat4 _MVP;
		GLuint _fieldTex;
		GLuint _offFieldTex;
		int _offZoomFactor;
		int _edgeDepth;

		FrameBufferContainer* _fbc;

		GLuint _query;
		GLuint _processedEdgeVBO;
		GLuint _vao[3];
		GLuint _vbo[2];
};


#endif