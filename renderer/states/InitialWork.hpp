#ifndef INITIAL_WORK_H
#define INITIAL_WORK_H

#include "../IRenderState.hpp"
#include "../../painter/GaussPainter.hpp"
#include "../PainterCommander.hpp"

class Renderer;

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
		void changeAspectRatio( void );

	private:
		enum GAUSS {VIEW};
		enum COMMANDER {GAUSS_VIEW};

		Renderer* _r;

		float _progress;
		GaussPainter* _gaussPainter[1];
		PainterCommander* _pc[1];
		

};


#endif