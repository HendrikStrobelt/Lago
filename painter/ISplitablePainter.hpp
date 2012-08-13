#ifndef ISPLITABLE_PAINTER_H
#define ISPLITABLE_PAINTER_H

#include <GL/glew.h>
#include <GL/glfw.h>

class ISplitablePainter {

	public:
		virtual ~ISplitablePainter( void ) {};
		
		virtual int getElementCount( void ) = 0;
		virtual void processElements(int start, int count) = 0;

		virtual GLuint getWorkingTexture( void ) = 0;
		virtual GLuint detachTexture( void ) = 0;


	private:

};

#endif