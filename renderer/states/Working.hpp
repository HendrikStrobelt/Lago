#ifndef WORKING_H
#define WORKING_H

#include "../IRenderState.hpp"
class Renderer;

class Working : public IRenderState {

	public:
		Working(Renderer* renderer);
		~Working( void );

		//Interface methods
		void render( void );
		void renderGauss( void );
		void renderEvalField( void );
		void work( void );
		void changePanning( void );
		void changeZoom( void );
		void changeData( void );
		void changeAspectRatio( void );

	private:
		Renderer* _renderer;

};


#endif