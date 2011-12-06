#ifndef INITIAL_WORK_H
#define INITIAL_WORK_H

#include "../IRenderState.hpp"

class Renderer;
class WorkStateHelper;

class InitialWork : public IRenderState {

	public:
		InitialWork(Renderer* renderer);
		~InitialWork( void );

		//Interface methods
		void render( void );
		void renderGauss( void );
		void renderEvalField( void );
		void work( void );
		void takeOver( void );
		void changePanning( void );
		void changeZoom( void );
		void changeData(string nodeFile, string edgeFile="");
		void changeSideLength( void );
		void changeWindow( void );

	private:
		Renderer* _r;	
		WorkStateHelper* _worker;
};


#endif