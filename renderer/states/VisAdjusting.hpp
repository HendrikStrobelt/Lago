#ifndef VIS_ADJUSTING_H
#define VIS_ADJUSTING_H

#include "../IRenderState.hpp"
#include "../../painter/VisPainter.hpp"
#include "../RenderData.hpp"

class Renderer;

class VisAdjusting : public IRenderState {

	public:
		VisAdjusting(Renderer* renderer);
		~VisAdjusting( void );

		//Interface methods
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
		void swap( void );

		GLuint _blendVis;
		RenderData* _blendRenderData;

		Renderer* _r;
		VisPainter* _visPainter;
		float _blendFactor;
		bool _blendChange;
};


#endif