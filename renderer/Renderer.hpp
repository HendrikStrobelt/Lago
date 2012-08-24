#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <GL/glfw.h>

#include "..\DataStore.hpp"
#include "RenderData.hpp"
#include "IRenderData.hpp"
#include "IRenderState.hpp"
#include "states\Idle.hpp"
#include "states\InitialWork.hpp"
#include "states\Working.hpp"
#include "states\VisAdjusting.hpp"

#include "..\painter\ProgressbarPainter.hpp"
#include "..\painter\DisplayConvertPainter.hpp"
#include "..\painter\ScalingPainter.hpp"
#include "..\painter\LabelSelectionPainter.hpp"
#include "..\painter\LabelPainter.hpp"
#include "TextureExaminer.hpp"
#include "CellLabelGetter.hpp"

class Renderer : public IRenderState {

	friend class Idle;
	friend class InitialWork;
	friend class Working;
	friend class VisAdjusting;
	friend class WorkStateHelper;


	public:
		Renderer( void );
		~Renderer( void );

		void emptyClick( void );
		void rightClick(int x, int y);
		void labelClick(bool add, int id);
		void lockClick( void );

		//State methods
		void render( void );
		void renderGauss( void );
		void renderEvalField( void );
		void renderLineField( void );
		void work( void );
		void takeOver( void );
		void changePanning(int xMouseMove, int yMouseMove);
		void changeZoom( void );
		void changeData( void );
		void changeSideLength( void );
		void changeWindow( void );
		void changeVisParameter( void );

		void setNewData(string nodeFile, string edgeFile, bool silent=false);
		bool _hasEdges;

		bool _meassuring;
		int _meassureCount;
		double _time;
		double _timeSum;
	private:

		DataStore _dStore;

		RenderData* _currentData;
		RenderData* _newData;
		IRenderState* _state;
		GLuint _nodeVBO;
		GLuint _edgeVBO;
		GLuint _edgeProcessedVBO;

		int _windowWidth;
		int _windowHeight;

		int _mouseMoveX; //relativ to last
		int _mouseMoveY; //calculated result

		Idle* _idle;
		InitialWork* _initalWork;
		Working* _working;
		VisAdjusting* _visAdjust;
		vector<Node>* _initNodes;

		//Painter
		ProgressbarPainter _progressBar;
		ScalingPainter _scalingBars;
		DisplayConvertPainter _displayConvert;
		TextureExaminer _textureExaminer;
		LabelSelectionPainter _labelSelectionPainter;
		CellLabelGetter* _cellLabelGetter;
		LabelPainter _labelPainter;
		//

		glm::mat4 getStandardMVP( void );

		void setState(IRenderState* state);
		void renderGraph(IRenderData* rData, int xMove=0, int yMove=0);
		void renderHUD(float progress, int pBars, float maxVals[]);
		void renderLabelSelection(IRenderData* rData, int xMove=0, int yMove=0);
		void renderLabels(IRenderData* rData, int xMove, int yMove);
		void renderTexture(GLuint tex, float max[], float xMove=0.0f, float yMove=0.0f);
		void calculateMaxValues(float result[], GLuint texture, int textureWidth, int textureHeight);
};


#endif