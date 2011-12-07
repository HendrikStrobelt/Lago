#ifndef IDLE_H
#define IDLE_H

#include "../IRenderState.hpp"
class Renderer;

class Idle : public IRenderState {

	public:
		Idle(Renderer* renderer);
		~Idle( void );

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

	private:
		Renderer* _r;

};


#endif