#include "Context.hpp"

#include <vector>
#include <iostream>

#include "../GlobalConstants.hpp"
#include "../helper/CameraHelper.hpp"

namespace context {

	using namespace std;

	//push/pull read and write no notification
	bool _run;
	float _pixelSize;
	Options _options;
	ScaleOptions _scaleOptions[3];

	//pull
	float _zoomFactor;
	int _zoomExponent;
	int _sideExponent;

	float _worldTransX;
	float _worldTransY;

	Renderer* _renderer;

	//private var & methods
	double _pendingWheelEventTime;
	
	void clearRenderVariables( void );

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
		clearRenderVariables();
		addKeyEventListener(&_options);
		_options.init();
		_renderer = new Renderer;
	}

	void cleanUp() {
		delete _renderer;
	}


	void tick( void ) {
		if (_pendingWheelEventTime > 0 && _pendingWheelEventTime < glfwGetTime()) {
			int pos = glfwGetMouseWheel();
			for (unsigned int i = 0; i < _mouseWheelListeners.size(); i++) {
				_mouseWheelListeners[i]->mouseWheelEvent(pos);
			}
			_pendingWheelEventTime = -1.0;
		}
	}

	void clearRenderVariables( void ) {
		_zoomFactor = 1.0f;
		_zoomExponent = 0.0f;
		_sideExponent = 8;
		_worldTransX = 0.0f;
		_worldTransY = 0.0f;
		_pendingWheelEventTime = -1.0;
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

	double getTime( void ) {
		return glfwGetTime();
	}

	//SETTER

	void emptyClick( void ) {
		_renderer->emptyClick();
		//cinema
		cout << "empty click" << "\n";
	}

	void rightClick(int x, int y) {
		_renderer->rightClick(x, y);
		//cinema
		cout << "right click x: " << x << " y: " << y << "\n";
	}

	void labelClick(bool add, int id) {
		_renderer->labelClick(add, id);
		//cinema
		cout << "label click add: " << add << " id: " << id << "\n";
	}

	void lockClick( void ) {
		if (_options._lock) {
			_options._lock = false;
			_options._overLock = false;
			_renderer->changeVisParameter();
		} else {
			_options._lock = true;
		}
		_options._changedLocal = true;
		_renderer->lockClick();
	}

	void setSideExponent(int newSideExponent) {
		_sideExponent = newSideExponent;
		if (_sideExponent < 0) {
			_sideExponent = 0;
		}
		_renderer->changeSideLength();
	}

	void setZoomExponent(int newZoomExponent) {
		_zoomExponent = newZoomExponent;
		_zoomFactor = pow(ZOOM_BASE, _zoomExponent);

		//zoom to mouse
		if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
			int w,h;
			glfwGetWindowSize(&w, &h);
			int x,y;
			glfwGetMousePos(&x, &y);

			updateWorldTranslate((w/2)-x, (h/2)-y); //triggers panning update
		}
		_renderer->changeZoom();
	}

	void setDataSet(string nodeFile, string edgeFile) {
		clearRenderVariables();
		_renderer->changeData(nodeFile, edgeFile);
	}

	void visParameterChange( void ) {
		_renderer->changeVisParameter();
	}

	void updateWorldTranslate(int xMouseMove, int yMouseMove) {
		float worldMoveX, worldMoveY;
		cameraHelper::mouseDist2WorldDist(&worldMoveX, &worldMoveY, xMouseMove, yMouseMove);

		_worldTransX += worldMoveX;
		_worldTransY += worldMoveY;

		_renderer->changePanning(xMouseMove, yMouseMove);
	}

	//LISTENER WORKERS
	void resizeEvent(int width, int height) {
		if (_run) {
			for (unsigned int i = 0; i < _resizeListeners.size(); i++) {
				_resizeListeners[i]->resizeEvent(width, height);
			}
			_renderer->changeWindow();
		}
	}

	void mouseMovedEvent(int x, int y) {
		if (_run) {
			for (unsigned int i = 0; i < _mouseMotionListeners.size(); i++) {
				_mouseMotionListeners[i]->mouseMovedEvent(x, y);
			}
		}
	}

	void mouseClickedEvent(int button, int action) {
		if (_run) {
			for (unsigned int i = 0; i < _mouseClickListeners.size(); i++) {
				_mouseClickListeners[i]->mouseClickedEvent(button, action);
			}
		}
	}

	void mouseWheelEvent(int pos) {
		if (_run) {
			_pendingWheelEventTime = glfwGetTime() + 0.1; //100ms in the future
		}
	}

	void keyEvent(int key, int action) {
		if (_run) {
			for (unsigned int i = 0; i < _keyEventListeners.size(); i++) {
				_keyEventListeners[i]->keyEvent(key, action);
			}
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