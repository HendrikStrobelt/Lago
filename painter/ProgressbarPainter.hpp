#ifndef PROGRESS_BAR_PAINTER_H
#define PROGRESS_BAR_PAINTER_H

#include "../helper/GLSLShader.hpp"

namespace ProgressbarPainter {

	void init( void );
	void cleanUp( void );

	void renderBar(float loaded); //0..1

};

#endif