#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <GL/glfw.h>

#include "RenderData.hpp"
#include "IRenderState.hpp"
#include "states\Idle.hpp"
#include "states\InitialWork.hpp"
#include "states\Working.hpp"

class Renderer : public IRenderState {

	public:
		Renderer( void );
		~Renderer( void );

		//State methods
		void render( void );
		void renderGauss( void );
		void renderEvalField( void );
		void work( void );
		void changePanning( void );
		void changeZoom( void );
		void changeData( void );
		void changeAspectRatio( void );

	private:
		RenderData* _currentData;
		RenderData* _newData;
		IRenderState* _state;

		Idle* _idle;
		InitialWork* _initalWork;
		Working* _working;

		void renderGraph( void );
		void renderHUD(float progress);
		void renderTexture(GLuint tex);
};


#endif