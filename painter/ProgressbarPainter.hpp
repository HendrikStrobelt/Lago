#ifndef PROGRESS_BAR_PAINTER_H
#define PROGRESS_BAR_PAINTER_H

#include "../helper/GLSLShader.hpp"
#include "../context/IContextListener.hpp"

class ProgressbarPainter : IContextListener {

	public:
		ProgressbarPainter( void );
		~ProgressbarPainter( void );
		
		void renderBar(float loaded); //0..1
		
		//interface method
		void resizeEvent(int width, int height);

	private:
		enum DATA {VERTEX, TEX_COORDS};

		void updateBarData(int w, int h);

		void createShader( void );
		void initVao( void );

		GLSLShader* _shader_ptr;
		GLuint _vao;
		GLuint _vbo[2];
		GLuint _texLoadedBar;
		GLuint _texBar;

};

#endif