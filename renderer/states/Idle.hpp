#ifndef IDLE_H
#define IDLE_H

#include "../IRenderState.hpp"

class Idle : public IRenderState {

	public:
		Idle( void );
		~Idle( void );

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


};


#endif