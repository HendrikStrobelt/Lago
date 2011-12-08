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
		DividedLinePainter(GLuint edgeVBO, int width, int height);
		~DividedLinePainter( void );

		//interface methods
		int getElementCount( void );
		void processElements(int start, int count);

		GLuint getWorkingTexture( void );
		GLuint detachResultTexture( void );

		//static clean up
		static void cleanUp( void );

	private:
		enum VBO {UNITE_VERTEX, UNITE_TEX};

		static GLSLShader* _r_shader_ptr;
		static GLSLShader* _u_shader_ptr;
		static void createShader( void );
		
		void initVao(GLuint edgeVBO);

		FrameBufferContainer* _fbc;
		float _sideFactor;

		GLuint _vao[2];
		GLuint _vbo[2];
};


#endif