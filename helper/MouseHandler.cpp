#include "MouseHandler.hpp"
#include "../context/Context.hpp";

namespace mouseHandler {

	struct ClickBox {
		float bottom;
		float left;
		float up;
		float right;
		int lbVal;
		ClickBox(float pBottom, float pLeft, float pUp, float pRight, int pLbVal) : bottom(pBottom), left(pLeft), up(pUp), right(pRight), lbVal(pLbVal) {};
	};

	vector<ClickBox> _buttons;

	//private vars
	context::DummyContextListener _contextListener;
	bool _leftDown;
	int _xPressStart;
	int _yPressStart;


	//private methods
	bool testClick(float x, float y);
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
			
			*xMove = x - _xPressStart;
			*yMove = y - _yPressStart;
		}
	}

	//private
	void mouseClick(int button, int action) {
		if (action == GLFW_PRESS) {
			if (button == GLFW_MOUSE_BUTTON_LEFT) {
				int xPos, yPos, w, h;
				glfwGetMousePos(&xPos, &yPos);
				glfwGetWindowSize(&w, &h);

				bool registeredClick = testClick((float)xPos / (float)w, 1.0f - (float)yPos / (float)h);

				if (!registeredClick) {
					//start panning
					_leftDown = true;
					_xPressStart = xPos;
					_yPressStart = yPos;
				}
			}
		} else 
		if (action == GLFW_RELEASE) {
			int x,y;
			glfwGetMousePos(&x, &y);
			
			if (button == GLFW_MOUSE_BUTTON_LEFT) {
				if (_leftDown) {
					_leftDown = false;
	
					int xMove = x - _xPressStart;
					int yMove = y - _yPressStart;
					if (xMove != 0 || yMove != 0) {
						context::updateWorldTranslate(xMove, yMove);
					}
				}
			} else if (button == GLFW_MOUSE_BUTTON_RIGHT) {		
				context::rightClick(x, y);
			}
		}
	}
	
	
	void mouseWheelEvent(int pos) {
		context::setZoomExponent(context::_zoomExponent + pos);

		if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
			context::setSideExponent(context::_sideExponent + pos);
		}

		glfwSetMouseWheel(0);
	}



	//click functionality


	void registerLB(float bottom, float left, float up, float right, int lbVal) {
		_buttons.push_back(ClickBox(bottom, left, up, right, lbVal));
	}

	void clearLB( void ) {
		_buttons.clear();
	}


	//private
	bool testClick(float x, float y) {
		bool hit = false;
		
		for (int i = 0; i < _buttons.size(); i++) {
			if (   x > _buttons[i].left && x < _buttons[i].right
				&& y > _buttons[i].bottom && y < _buttons[i].up) {
					hit = true;
					context::labelClick(true, _buttons[i].lbVal);
					break;
			}
		}
				
		return hit;
	}

};