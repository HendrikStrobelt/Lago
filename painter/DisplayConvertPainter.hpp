#ifndef DISPLAY_CONVERT_PAINTER_H
#define DISPLAY_CONVERT_PAINTER_H

#include "../helper/GLSLShader.hpp"

class DisplayConvertPainter {

	public:
		DisplayConvertPainter( void );
		~DisplayConvertPainter( void );
		
		void renderTexture(GLuint texture, float max[], float xMove=0.0f, float yMove=0.0f);

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
};

#endif