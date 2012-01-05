#ifndef VIS_ADJUSTING_H
#define VIS_ADJUSTING_H

#include "../IRenderState.hpp"
#include "../../painter/VisPainter.hpp"

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
		void mixMaxVals(float ret[], float curVals[], float newVals[], float process);

		Renderer* _r;
		float _process;
		double _animationStart;
		VisPainter* _visPainter;
};


#endif