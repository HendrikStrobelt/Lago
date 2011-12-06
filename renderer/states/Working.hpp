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
		void work( void );
		void takeOver( void );
		void changePanning( void );
		void changeZoom( void );
		void changeData(string nodeFile, string edgeFile="");
		void changeSideRatio( void );

	private:
		Renderer* _r;
		WorkStateHelper* _worker;

};


#endif