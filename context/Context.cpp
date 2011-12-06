#include "Context.hpp"

#include <vector>
#include <iostream>


namespace context {

	using namespace std;

	//push/pull read and write no notification
	bool _run;
	ScaleOptions _scaleOptions[2];

	//pull
	float _sideRatio;
	float _userZoomFactor;
	Renderer* _renderer;

	//private methods
	void resizeEvent(int width, int height);
	void mouseMovedEvent(int x, int y);
	void mouseClickedEvent(int button, int action);
	void mouseWheelEvent(int pos);
	void keyEvent(int key, int action);

	vector<IContextListener* > _resizeListeners;
	vector<IContextListener* > _mouseMotionListeners;
	vector<IContextListener* > _mouseClickListeners;
	vector<IContextListener* > _mouseWheelListeners;
	vector<IContextListener* > _keyEventListeners;

	//INIT

	void init() {
		_run = true;
		_userZoomFactor = 1.0f;
		_sideRatio = 0.01f;
		_renderer = new Renderer;
	}

	void cleanUp() {
		delete _renderer;
	}

	void getWindowSize(int* width, int* height) {
		glfwGetWindowSize(width, height);
	}

	void getMousePos(int* x, int* y) {
		glfwGetMousePos(x, y);
	}

	int getMouseWheelValue( void ) {
		return glfwGetMouseWheel();
	}

	//SETTER

	void setSideRatio(float newSideRatio) {
		_sideRatio = newSideRatio;
		_renderer->changeSideRatio();
	}

	void setDataSet(string nodeFile, string edgeFile) {
		_renderer->changeData(nodeFile, edgeFile);
	}

	//LISTENER WORKERS
	void resizeEvent(int width, int height) {
		for (unsigned int i = 0; i < _resizeListeners.size(); i++) {
			_resizeListeners[i]->resizeEvent(width, height);
		}
	}

	void mouseMovedEvent(int x, int y) {
		for (unsigned int i = 0; i < _mouseMotionListeners.size(); i++) {
			_mouseMotionListeners[i]->mouseMovedEvent(x, y);
		}
	}

	void mouseClickedEvent(int button, int action) {
		for (unsigned int i = 0; i < _mouseClickListeners.size(); i++) {
			_mouseClickListeners[i]->mouseClickedEvent(button, action);
		}
	}

	void mouseWheelEvent(int pos) {
		for (unsigned int i = 0; i < _mouseWheelListeners.size(); i++) {
			_mouseWheelListeners[i]->mouseWheelEvent(pos);
		}
	}

	void keyEvent(int key, int action) {
		for (unsigned int i = 0; i < _keyEventListeners.size(); i++) {
			_keyEventListeners[i]->keyEvent(key, action);
		}
	}

	

	//REGISTER LISTENERS

	void addResizeListener(IContextListener* listener) {
		_resizeListeners.push_back(listener);
		if (_resizeListeners.size() == 1) {//added first value
			glfwSetWindowSizeCallback(resizeEvent);
		}
	}

	void addMouseMovedListener(IContextListener* listener) {
		_mouseMotionListeners.push_back(listener);
		if (_mouseMotionListeners.size() == 1) {//added first value
			glfwSetMousePosCallback(mouseMovedEvent);
		}
	}

	void addMouseClickedListener(IContextListener* listener) {
		_mouseClickListeners.push_back(listener);
		if (_mouseClickListeners.size() == 1) {//added first value
			glfwSetMouseButtonCallback(mouseClickedEvent);
		}
	}

	void addMouseWheelListener(IContextListener* listener) {
		_mouseWheelListeners.push_back(listener);
		if (_mouseWheelListeners.size() == 1) {//added first value
			glfwSetMouseWheelCallback(mouseWheelEvent);
		}
	}

	void addKeyEventListener(IContextListener* listener) {
		_keyEventListeners.push_back(listener);
		if (_keyEventListeners.size() == 1) {//added first value
			glfwSetKeyCallback(keyEvent);
		}
	}


	//CLASS DUMMY CONTEXT LISTENER

	DummyContextListener::DummyContextListener( void ) {

	}

	void DummyContextListener::activate(resizedFun resize, mouseMovedFun mouseMove, mouseClickedFun mouseClick,
								 keyEventFun keyEve, mouseWheelFun mouseWheel) {

			//register the appropriate listeners
			if (resize != 0) {
				_resize = resize;
				context::addResizeListener(this);
			}
			if (mouseMove != 0) {
				_mouseMove = mouseMove;
				context::addMouseMovedListener(this);
			}
			if (mouseClick != 0) {
				_mouseClick = mouseClick;
				context::addMouseClickedListener(this);
			}
			if (keyEve != 0) {
				_keyEve = keyEve;
				context::addKeyEventListener(this);
			}
			if (mouseWheel != 0) {
				_mouseWheel = mouseWheel;
				context::addMouseWheelListener(this);
			}
	}

	//implement interface
	void DummyContextListener::resizeEvent(int width, int height) {
		_resize(width, height);
	}
	
	void DummyContextListener::mouseMovedEvent(int x, int y) { 
		_mouseMove(x, y);
	}

	void DummyContextListener::mouseClickedEvent(int button, int action) {
		_mouseClick(button, action);
	}

	void DummyContextListener::mouseWheelEvent(int pos) {
		_mouseWheel(pos);
	}

	void DummyContextListener::keyEvent(int key, int action) {
		_keyEve(key, action);
	}

};