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
#include "..\painter\DisplayConvertPainter.hpp"
#include "TextureExaminer.hpp"

class Renderer : public IRenderState {

	friend class Idle;
	friend class InitialWork;
	friend class Working;

	public:
		Renderer( void );
		~Renderer( void );

		void setNewData(string nodeFile, string edgeFile = "");

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

		int _windowWidth;
		int _windowHeight;

		Idle* _idle;
		InitialWork* _initalWork;
		Working* _working;

		//Painter
		ProgressbarPainter _progressBar;
		DisplayConvertPainter _displayConvert;
		TextureExaminer _textureExaminer;
		//
		
		void renderGraph( void );
		void renderHUD(float progress);
		void renderTexture(GLuint tex, float rMax=1.0f, float gMax=1.0f, float bMax=1.0f);
		void calculateMaxValues(float result[], GLuint texture, int textureWidth, int textureHeight);
};


#endif