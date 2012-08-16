#ifndef WORKING_H
#define WORKING_H

#include "../IRenderState.hpp"

class Renderer;
class WorkStateHelper;

class Working : public IRenderState {

	public:
		Working(Renderer* renderer);
		~Working( void );

		//Interface methods
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

	private:
		Renderer* _r;
		WorkStateHelper* _worker;
};


#endif