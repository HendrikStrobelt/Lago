#ifndef VIS_PAINTER_H
#define VIS_PAINTER_H

#include <glm/glm.hpp>
#include "../helper/GLSLShader.hpp"
#include "../renderer/RenderData.hpp"

class VisPainter {

	public:
		VisPainter( void );
		~VisPainter( void );

		void renderVis(RenderData* rData);

		//static clean up
		static void cleanUp( void );
	private:
		enum DATA {VERTEX, TEX_COORDS};
		
		static void createShader( void );
		static GLSLShader* _shader_ptr;

		void initVao( void );

		GLuint _vao;
		GLuint _vbo[2];
};

#endif