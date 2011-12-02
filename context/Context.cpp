#include "Context.hpp"

#include <vector>
#include <iostream>


namespace context {

	using namespace std;

	//push/pull read and write no notification
	bool _run;
	ScaleOptions _scaleOptions[2];
	float _pixelSize;

	//pull
	double _renderTime;

	//private vars
	float _sigma;

	//private methods
	void dataChangedEvent( void );
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
	vector<IContextListener* > _sigmaListeners;
	vector<IContextListener* > _dataListeners;
	vector<IContextListener* > _joinDepthListeners;

	//INIT

	void init() {
		_sigma = 1.0f; //tmp value overwritten by CaneraOrtho
		_run = true;
	}

	void cleanUp() {
	}

	//GET

	void getWindowSize(int* width, int* height) {
		glfwGetWindowSize(width, height);
	}

	void getMousePos(int* x, int* y) {
		glfwGetMousePos(x, y);
	}

	int getMouseWheelValue( void ) {
		return glfwGetMouseWheel();
	}

	float getSigma( void ) {
		return _sigma;
	}


	//SET

	void setSigma(float sigma) {
		if ((sigma*2) < context::_pixelSize) {
			sigma = context::_pixelSize / 2.0f;
		}
		
		_sigma = sigma;
		for (unsigned int i = 0; i < _sigmaListeners.size(); i++) {
			_sigmaListeners[i]->sigmaChanged();
		}
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
		

	void dataChangedEvent( void ) {
		for (unsigned int i = 0; i < _dataListeners.size(); i++) {
			_dataListeners[i]->dataChanged();
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

	void addSigmaChangedListener(IContextListener* listener) {
		_sigmaListeners.push_back(listener);
	}

	void addDataChangedListener(IContextListener* listener) {
		_dataListeners.push_back(listener);
	}

	//CLASS DUMMY CONTEXT LISTENER

	DummyContextListener::DummyContextListener( void ) {

	}

	void DummyContextListener::activate(resizedFun resize, mouseMovedFun mouseMove, mouseClickedFun mouseClick,
								 keyEventFun keyEve, sigmaChangedFun sigmaChange, dataChangedFun dataChange,
								 mouseWheelFun mouseWheel) {

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
			if (sigmaChange != 0) {
				_sigmaChange = sigmaChange;
				context::addSigmaChangedListener(this);
			}
			if (dataChange != 0) {
				_dataChange = dataChange;
				context::addDataChangedListener(this);
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

	void DummyContextListener::sigmaChanged( void ) {
		_sigmaChange();
	}

	void DummyContextListener::dataChanged( void ) {
		_dataChange();
	}

};