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

		//fbo has to be bound
		void setAttachment0NoDeleteNoBind(GLuint newAttachment); //replaces the 0 attachment and changes _fboOutTex does not delete the previous bound texture!!!
		void addStencilBuffer( void );

		GLuint detachTexture( void );

		//pull only!
		GLuint _fbo;
		GLuint _fboOutTex;

	private:

		//needed for the texture recreation when deataching
		int _bufferWidth;
		int _bufferHeight;

		bool _hasStencilBuffer;
		GLenum _textureFilter;
		GLenum _type;
		GLenum _internalFormat;
		GLuint _stencilBuffer;

		void prepareFBO( void );

};


#endif