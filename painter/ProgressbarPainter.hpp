#ifndef PROGRESS_BAR_PAINTER_H
#define PROGRESS_BAR_PAINTER_H

#include "../helper/GLSLShader.hpp"
#include "../context/IContextListener.hpp"

class ProgressbarPainter : public IContextListener {

	public:
		ProgressbarPainter( void );
		~ProgressbarPainter( void );
		
		void renderBar(float loaded); //0..1

		//interface method
		void resizeEvent(int width, int height);

		//static clean up
		static void cleanUp( void );
	private:
		enum DATA {VERTEX, TEX_COORDS};
		
		static GLSLShader* _shader_ptr;
		static void createShader( void );
		
		void updateBarPosition(int w, int h);

		void getShader( void );
		void initVao( void );

		GLuint _vao;
		GLuint _vbo[2];
		GLuint _texLoadedBar;
		GLuint _texBar;

};

#endif