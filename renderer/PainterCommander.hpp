#ifndef PAINTER_COMMANDER_H
#define PAINTER_COMMANDER_H

#include "../helper/FrameBufferContainer.hpp"
#include "../painter/ISplitablePainter.hpp"
#include <GL\glfw.h>
#include <GL\glew.h>

class PainterCommander {

	#define TARGET_RENDER_TIME (1.0 / 10.0)
	#define INIT_STEP 500

	public:
		PainterCommander(ISplitablePainter* painter, int width, int height, int initStep);
		~PainterCommander( void );

		float renderNextPart( void ); //0..1 completion
		double getRenderTime( void );
		bool isDone( void );

		GLuint getWorkingTexture( void );
		GLuint detachResult( void );

	private:
		FrameBufferContainer* _fbc;
		ISplitablePainter* _painter;
		double _renderTime;
		int _currentIndex;
		int _currentStep;
};

#endif;