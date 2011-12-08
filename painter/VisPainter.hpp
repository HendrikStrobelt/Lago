#ifndef VIS_PAINTER_H
#define VIS_PAINTER_H

#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include "../helper/GLSLShader.hpp"
#include "../renderer/RenderData.hpp"
#include "../helper/FrameBufferContainer.hpp"

class VisPainter {

	public:
		VisPainter( void );
		~VisPainter( void );

		void renderVis(RenderData* rData, bool withEdges, float moveX, float moveY);
		
		void resize(int width, int height);

		//static clean up
		static void cleanUp( void );
	private:
		enum DATA {VERTEX, TEX_COORDS};
		
		static void createShader( void );
		static GLSLShader* _n_shader_ptr;
		static GLSLShader* _e_shader_ptr;

		void renderNodes(RenderData* rData, float moveX, float moveY);
		void renderEdges(RenderData* rData, GLuint nodeTex, float moveX, float moveY);
		void initVao( void );

		int _width;
		int _height;

		FrameBufferContainer* _fbc;

		GLuint _vao;
		GLuint _vbo[2];
};

#endif