#ifndef VIS_PAINTER_H
#define VIS_PAINTER_H

#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include "../helper/GLSLShader.hpp"
#include "../renderer/IRenderData.hpp"
#include "../helper/FrameBufferContainer.hpp"

class VisPainter {

	public:
		VisPainter(int width, int height);
		~VisPainter( void );

		bool isDone( void );

		GLuint detachResult( void );
		void renderVis(IRenderData* rData, bool withEdges, bool animation=false, IRenderData* oldData=NULL, glm::mat4 MVP=glm::mat4(1.0f), float process=-1.0f);

		//static clean up
		static void cleanUp( void );
	private:
		enum DATA {VERTEX, TEX_COORDS, VERTEX2};

		static void createShader( void );

		static GLSLShader* _n_shader_ptr;
		static GLSLShader* _e_shader_ptr;

		void renderNodes(IRenderData* rData, bool animation=false, IRenderData* oldData=NULL, glm::mat4 MVP=glm::mat4(1.0f), float process=-1.0f);
		void renderEdges(IRenderData* rData, GLuint nodeTex);
		void initVao( void );

		bool _done;

		int _width;
		int _height;

		FrameBufferContainer* _fbcInter;
		FrameBufferContainer* _fbcRes;

		GLuint _vao;
		GLuint _vbo[2];
};

#endif