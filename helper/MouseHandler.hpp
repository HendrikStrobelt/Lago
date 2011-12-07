#ifndef MOUSE_HANDLER_H
#define MOUSE_HANDLER_H

namespace mouseHandler {

	void init( void );
	void cleanUp( void );

	void getPressMovement(int* xMove, int* yMove);

};


#endif