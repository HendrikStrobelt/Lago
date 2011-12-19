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
#include "states\VisAdjusting.hpp"

#include "..\painter\ProgressbarPainter.hpp"
#include "..\painter\DisplayConvertPainter.hpp"
#include "..\painter\ScalingPainter.hpp"
#include "TextureExaminer.hpp"

class Renderer : public IRenderState {

	friend class Idle;
	friend class InitialWork;
	friend class Working;
	friend class VisAdjusting;
	friend class WorkStateHelper;


	public:
		Renderer( void );
		~Renderer( void );

		//State methods
		void render( void );
		void renderGauss( void );
		void renderEvalField( void );
		void renderLineField( void );
		void work( void );
		void takeOver( void );
		void changePanning(int xMouseMove, int yMouseMove);
		void changeZoom( void );
		void changeData(string nodeFile, string edgeFile="");
		void changeSideLength( void );
		void changeWindow( void );
		void changeVisParameter( void );

	private:

		DataCache _dCache;

		RenderData* _currentData;
		RenderData* _newData;
		IRenderState* _state;
		GLuint _nodeVBO;
		GLuint _edgeVBO;

		bool _hasEdges;

		int _windowWidth;
		int _windowHeight;

		int _mouseMoveX; //relativ to last
		int _mouseMoveY; //calculated result

		Idle* _idle;
		InitialWork* _initalWork;
		Working* _working;
		VisAdjusting* _visAdjust;

		//Painter
		ProgressbarPainter _progressBar;
		ScalingPainter _scalingBars;
		DisplayConvertPainter _displayConvert;
		TextureExaminer _textureExaminer;
		//

		void setNewData(string nodeFile, string edgeFile="");
		void updateLabels(RenderData* rData);

		void setState(IRenderState* state);
		void renderGraph(RenderData* rData, int xMove=0, int yMove=0);
		void renderHUD(float progress, float maxVals[]);
		void renderLabels(RenderData* rData, int xMove, int yMove);
		void renderTexture(GLuint tex, float max[], float xMove=0.0f, float yMove=0.0f);
		void calculateMaxValues(float result[], GLuint texture, int textureWidth, int textureHeight);
};


#endif