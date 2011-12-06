#ifndef RENDER_DATA_H
#define RENDER_DATA_H

#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>

class RenderData {

	public:
		RenderData( void );
		~RenderData( void );

		void clear( void );

		GLuint _gaussTex;
		GLuint _evalField;
		GLuint _lineField;
		
		glm::mat4 _P;
		glm::mat4 _T;

		float _aspectRatio;
		int _dataID;
		float _maxValuesN[3];
		float _maxValuesE[3];

	private:

};


#endif