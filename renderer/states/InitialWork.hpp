#ifndef INITIAL_WORK_H
#define INITIAL_WORK_H

#include "../IRenderState.hpp"
#include "../../helper/FrameBufferContainer.hpp"
#include "../../painter/GaussPainter.hpp"

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
		void changePanning( void );
		void changeZoom( void );
		void changeData( void );
		void changeAspectRatio( void );

	private:
		Renderer* _renderer;
		GaussPainter* _gaussPainter;
		FrameBufferContainer* _fbo;
		

};


#endif