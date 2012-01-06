#ifndef IRENDER_DATA_H
#define IRENDER_DATA_H

#include <GL/glew.h>
#include <GL/glfw.h>

class IRenderData {

	public:
		virtual ~IRenderData() {};

		virtual void clear( void ) = 0;
		virtual GLuint getGaussTex( void ) = 0;
		virtual GLuint getEvalField( void ) = 0;
		virtual GLuint getLineField( void ) = 0;
		virtual GLuint getVis( void ) = 0;
		virtual float getNodeMax( void ) = 0;
		virtual float getEdgeMax( void ) = 0;

		virtual float* getNodeMaxAll( void ) = 0;
		virtual float* getEdgeMaxAll( void ) = 0;

		virtual float* getBox( void ) = 0;
		virtual float getSideLength( void ) = 0;
	private:

};


#endif