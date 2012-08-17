#ifndef IDLE_PAINTER_H
#define IDLE_PAINTER_H

#include "../helper/GLSLShader.hpp"
#include "../context/IContextListener.hpp"

class IdlePainter: public IContextListener {

	public:
		IdlePainter( void );
		~IdlePainter( void );
		
		void render();
		void resizeEvent(int width, int height);
		//static clean up
		static void cleanUp( void );
	private:
		enum DATA {VERTEX, TEX_COORDS};
		
		static GLSLShader* _shader_ptr;
		static void createShader( void );
		
	
		void getShader( void );
		void initVao( void );

		GLuint _vao;
		GLuint _vbo[2];
		GLuint _tex;

};

#endif