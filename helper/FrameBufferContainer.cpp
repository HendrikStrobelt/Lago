#include "FrameBufferContainer.hpp"

FrameBufferContainer::FrameBufferContainer(int bufferWidth, int bufferHeight) {
	_bufferWidth = bufferWidth;
	_bufferHeight = bufferHeight;
	_textureFilter = GL_NEAREST;
	_internalFormat = GL_RGBA32F;
	_type = GL_FLOAT;
	prepareFBO();
}

FrameBufferContainer::FrameBufferContainer(int bufferWidth, int bufferHeight, GLenum textureFilter) {
	_bufferWidth = bufferWidth;
	_bufferHeight = bufferHeight;
	_textureFilter = textureFilter;
	_internalFormat = GL_RGBA32F;
	_type = GL_FLOAT;
	prepareFBO();
}

FrameBufferContainer::FrameBufferContainer(int bufferWidth, int bufferHeight, GLenum textureFilter, GLenum internalFormat, GLenum type) {
	_bufferWidth = bufferWidth;
	_bufferHeight = bufferHeight;
	_textureFilter = textureFilter;
	_internalFormat = internalFormat;
	_type = type;
	prepareFBO();
}

FrameBufferContainer::~FrameBufferContainer( void ) {
	if (_hasStencilBuffer) {
		glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteRenderbuffers(1, &_stencilBuffer);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDeleteTextures(1, &_fboOutTex);
	glDeleteFramebuffers(1, &_fbo);
}

void FrameBufferContainer::setAttachment0NoDeleteNoBind(GLuint newAttachment) {
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, newAttachment, 0);
	_fboOutTex = newAttachment;
}

GLuint FrameBufferContainer::detachTexture( void ) {
	GLuint tmp = _fboOutTex;

	glGenTextures(1, &_fboOutTex);
	
	glBindTexture(GL_TEXTURE_2D, _fboOutTex);
		glTexImage2D(GL_TEXTURE_2D, 0, _internalFormat, _bufferWidth, _bufferHeight, 0, GL_RGBA, _type, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _textureFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _textureFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _fboOutTex, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return tmp;
}

void FrameBufferContainer::addStencilBuffer( void ) {
	glGenRenderbuffers(1, &_stencilBuffer);

	glBindRenderbuffer(GL_RENDERBUFFER, _stencilBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _bufferWidth, _bufferHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _stencilBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBufferContainer::prepareFBO( void ) {
	
	//internalFormat  =   //GL_RGBA32F;//GL_RGBA16;//GL_RGBA;//  
	//type = //GL_FLOAT;//GL_UNSIGNED_INT;//GL_UNSIGNED_BYTE;//

    glGenTextures(1, &_fboOutTex);
	
	glBindTexture(GL_TEXTURE_2D, _fboOutTex);
		glTexImage2D(GL_TEXTURE_2D, 0, _internalFormat, _bufferWidth, _bufferHeight, 0, GL_RGBA, _type, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _textureFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _textureFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

 
    // create FBO
    glGenFramebuffers(1, &_fbo);
	
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _fboOutTex, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// clear FBO
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
		glClearColor(0,0,0,1);
		glClear(GL_COLOR_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}