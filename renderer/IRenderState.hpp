#ifndef IRENDER_STATE_H
#define IRENDER_STATE_H

class IRenderState {

	public:
		virtual ~IRenderState( void ) {};

		virtual void render( void ) = 0;
		virtual void renderGauss( void ) = 0;
		virtual void renderEvalField( void ) = 0;

		virtual void work( void ) = 0;

		//state transissions
		virtual void changePanning( void ) = 0;
		virtual void changeZoom( void ) = 0;
		virtual void changeData( void ) = 0;
		virtual void changeAspectRatio( void ) = 0;

};

#endif