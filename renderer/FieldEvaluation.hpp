#ifndef FIELD_EVALUATION_H
#define FIELD_EVALUATION_H

#include <glm/glm.hpp>
#include "../helper/FrameBufferContainer.hpp"
#include "../helper/GLSLShader.hpp"

class FieldEvaluation {

	public:

		FieldEvaluation(int width, int height);
		~FieldEvaluation();

		void evaluate(GLuint texHandel);
		GLuint detachResultTexture( void );

		bool isDone( void );

		//static clean up
		static void cleanUp( void );

	private:	
		enum VBO {VERTEX, TEX_COORDS};

		static GLSLShader* _shader_ptr;
		static void createShader( void );
		void initVao( void );

		int _width;
		int _height;

		bool _done;

		FrameBufferContainer* _evalFBO;

		GLuint _vao;
		GLuint _vbo[2];
};

#endif