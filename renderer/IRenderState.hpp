#ifndef IRENDER_STATE_H
#define IRENDER_STATE_H

#include <string>

using namespace std;

class IRenderState {

	public:
		virtual ~IRenderState( void ) {};

		virtual void render( void ) = 0;
		virtual void renderGauss( void ) = 0;
		virtual void renderEvalField( void ) = 0;
		virtual void renderLineField( void ) = 0;

		virtual void work( void ) = 0;
		virtual void takeOver( void ) = 0;

		//state transissions
		virtual void changePanning(int xMouseMove, int yMouseMove) = 0;
		virtual void changeZoom( void ) = 0;
		virtual void changeData(string nodeFile, string edgeFile="") = 0;
		virtual void changeSideLength( void ) = 0;
		virtual void changeWindow( void ) = 0;
		virtual void changeVisParameter( void ) = 0;

};

#endif