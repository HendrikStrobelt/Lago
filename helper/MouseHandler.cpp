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
	vector<ClickBox> _labels;

	//private vars
	context::DummyContextListener _contextListener;
	bool _leftDown;
	bool _pressMovement;

	int _xClickStart;
	int _yClickStart;
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

		 int xPos, yPos;
 		 glfwGetMousePos(&xPos, &yPos);

		 if (_leftDown && !_pressMovement) {
			 float movement  = sqrt((float)(xPos-_xClickStart)*(xPos-_xClickStart) + (yPos-_yClickStart)*(yPos-_yClickStart));
			 if (movement > 6.0f) {
				 //moved far enough
				 _pressMovement = true;
				 _xPressStart = xPos;
				 _yPressStart = yPos;
			 }
		 }

		 if (_pressMovement) {
			*xMove = xPos - _xPressStart;
			*yMove = yPos - _yPressStart;
		 } else {
			*xMove = 0;
			*yMove = 0;
		 }
	}

	//private
	void mouseClick(int button, int action) {
		if (action == GLFW_PRESS) {
			if (button == GLFW_MOUSE_BUTTON_LEFT) {
				glfwGetMousePos(&_xClickStart, &_yClickStart);
				_leftDown = true;
			}
		} else 
		if (action == GLFW_RELEASE) {
			int x,y;
			glfwGetMousePos(&x, &y);
			
			if (button == GLFW_MOUSE_BUTTON_LEFT) {
				if (_leftDown) {			
					if (!_pressMovement) {
						//could be a click
						int w,h;
						glfwGetWindowSize(&w, &h);					
						bool click = testClick((float)x / (float)w, 1.0f - (float)y / (float)h);//reacts to clicks

						if (!click) {
							context::emptyClick();
						}
					} else {
						int xMove = x - _xPressStart;
						int yMove = y - _yPressStart;
						if (xMove != 0 || yMove != 0) {
							context::updateWorldTranslate(xMove, yMove);
						}
					}
					_leftDown = false;
					_pressMovement = false;
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

	void registerLabel(float bottom, float left, float up, float right, int lbVal) {
		_labels.push_back(ClickBox(bottom, left, up, right, lbVal));
	}

	void registerLB(float bottom, float left, float up, float right, int lbVal) {
		_buttons.push_back(ClickBox(bottom, left, up, right, lbVal));
	}

	void clearLB( void ) {
		_buttons.clear();
	}

	void clearLabel( void ) {
		_labels.clear();
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

		if (!hit) {
			for (int i = 0; i < _labels.size(); i++) {
				if (   x > _labels[i].left && x < _labels[i].right
					&& y > _labels[i].bottom && y < _labels[i].up) {
						hit = true;
						context::labelClick(false, _labels[i].lbVal);
						break;
				}
			}
		}
				
		return hit;
	}

};