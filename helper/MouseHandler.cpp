#include "MouseHandler.hpp"
#include "../context/Context.hpp";

namespace mouseHandler {

	//private vars
	context::DummyContextListener _contextListener;
	bool _leftDown;
	int _xPressStart;
	int _yPressStart;


	//private methods
	void mouseWheelEvent(int pos);
	void mouseClick(int button, int action);

	//public
	void init( void ) {
		_contextListener.activate(0, 0, mouseClick, 0, mouseWheelEvent);
		_leftDown = false;
	}

	void cleanUp( void ) {

	}

	void getPressMovement(int* xMove, int* yMove) {
		if (!_leftDown) {
			*xMove = 0;
			*yMove = 0;
		} else {
			int x,y;
			glfwGetMousePos(&x, &y);
			
			*xMove = _xPressStart - x;
			*yMove = _yPressStart - y;
		}
	}

	//private
	void mouseClick(int button, int action) {
		if (action == GLFW_PRESS) {
			if (button == GLFW_MOUSE_BUTTON_LEFT) {
				_leftDown = true;
				glfwGetMousePos(&_xPressStart, &_yPressStart);
			}
		} else 
		if (action == GLFW_RELEASE) {
			if (button == GLFW_MOUSE_BUTTON_LEFT) {
				_leftDown = false;
			}
		}
	}
	
	
	void mouseWheelEvent(int pos) {
		context::setZoomExponent(context::_zoomExponent + pos);
		glfwSetMouseWheel(0);
	}

};