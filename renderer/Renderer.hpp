#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <GL/glfw.h>

#include "..\DataCache.hpp"
#include "RenderData.hpp"
#include "IRenderState.hpp"
#include "states\Idle.hpp"
#include "states\InitialWork.hpp"
#include "states\Working.hpp"

#include "..\painter\ProgressbarPainter.hpp"

class Renderer : public IRenderState {

	friend class Idle;
	friend class InitialWork;
	friend class Working;

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
		DataCache dCache;

		RenderData* _currentData;
		RenderData* _newData;
		IRenderState* _state;
		GLuint _nodeVBO;

		Idle* _idle;
		InitialWork* _initalWork;
		Working* _working;

		//Painter
		ProgressbarPainter _progressBar;
		//

		void setNewData(string nodeFile, string edgeFile = "");
		void renderGraph( void );
		void renderHUD(float progress);
		void renderTexture(GLuint tex);
};


#endif