#ifndef FRAME_BUFFER_CONTAINER_H
#define FRAME_BUFFER_CONTAINER_H

#include <GL/glew.h>
#include <GL/glfw.h>

class FrameBufferContainer {

	public:
		FrameBufferContainer(int bufferWidth, int bufferHeight);
		FrameBufferContainer(int bufferWidth, int bufferHeight, GLenum textureFilter);
		FrameBufferContainer(int bufferWidth, int bufferHeight, GLenum textureFilter, GLenum internalFormat, GLenum type);
		~FrameBufferContainer( void );

		GLuint detachTexture( void );

		//pull only!
		GLuint _fbo;
		GLuint _fboOutTex;

	private:

		//needed for the texture recreation when deataching
		int _bufferWidth;
		int _bufferHeight;
		GLenum _textureFilter;
		GLenum _type;
		GLenum _internalFormat;

		void prepareFBO( void );

};


#endif