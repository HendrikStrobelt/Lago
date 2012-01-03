#ifndef MOUSE_HANDLER_H
#define MOUSE_HANDLER_H

namespace mouseHandler {

	void init( void );
	void cleanUp( void );

	void getPressMovement(int* xMove, int* yMove);

	void registerLB(float bottom, float left, float up, float right, int lbVal); //label and button clicks hard coded functionality
	void clearLB( void );
};


#endif