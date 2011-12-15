#ifndef SCALING_PAINTER_H
#define SCALING_PAINTER_H

#include "../helper/GLSLShader.hpp"
#include "../renderer/RenderData.hpp"
#include "../context/IContextListener.hpp"

class ScalingPainter : public IContextListener {

	public:
		ScalingPainter( void );
		~ScalingPainter( void );
		
		void renderScaleBars(RenderData* rData);

		//interface method
		void resizeEvent(int width, int height);

		//static clean up
		static void cleanUp( void );
	private:
		enum DATA {VERTEX, TEX_COORDS};
		
		static GLSLShader* _shader_ptr;
		static void createShader( void );
		
		void updateBarPositions(int w, int h);

		void getShader( void );
		void initVao( void );

		GLuint _vao;
		GLuint _vbo[2];
};

#endif