#ifndef RENDER_DATA_H
#define RENDER_DATA_H

#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include "../painter/LabelPainter.hpp"

class RenderData {

	public:
		RenderData( void );
		~RenderData( void );

		void clear( void );

		GLuint _gaussTex;
		GLuint _evalField;
		GLuint _lineField;
		GLuint _vis;

		float _maxValuesN[3];
		float _maxValuesE[3];

		LabelPainter _labelPainter;

	private:

};


#endif