#ifndef INITIAL_WORK_H
#define INITIAL_WORK_H

#include "../IRenderState.hpp"

class InitialWork : public IRenderState {

	public:
		InitialWork( void );
		~InitialWork( void );

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