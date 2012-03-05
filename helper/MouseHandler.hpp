#ifndef MOUSE_HANDLER_H
#define MOUSE_HANDLER_H

namespace mouseHandler {

	void init( void );
	void cleanUp( void );

	void getPressMovement(int* xMove, int* yMove);

	//label buttons
	void registerLB(float bottom, float left, float up, float right, int lbVal); //labelbutton clicks hard coded functionality
	void clearLB( void );
	//labels
	void registerLabel(float bottom, float left, float up, float right, int lbVal); //label clicks hard coded functionality
	void clearLabel( void );
	//lock
	void setLock(float bottom, float left, float up, float right, float xShift);

	//cinema
	void setMove(int xMovement, int yMovement);
};


#endif