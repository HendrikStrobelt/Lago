#ifndef CONTEXT_H
#define CONTEXT_H

#include "GL\glfw.h"
#include "IContextListener.hpp"
#include "ScaleOptions.hpp"


//context can only be used after calling init and after
//the glfw environment has been set up successfully
namespace context {

	void init();
	void cleanUp( void );

	//push/pull read and write no notification
		extern bool _run;
		extern ScaleOptions _scaleOptions[];
		extern float _pixelSize;

	//pull
		extern double _renderTime;

	//push stuff changable via set methods register a listener or ask
		//GLFW getters
		void getWindowSize(int* width, int* height);
		void getMousePos(int* x, int* y);
		int getMouseWheelValue( void );

		//program getters
		float getSigma( void );

		//program setters
		void setSigma(float sigma);

		//register listeners
		void addResizeListener(IContextListener* listener);
		void addMouseMovedListener(IContextListener* listener);
		void addMouseClickedListener(IContextListener* listener);
		void addMouseWheelListener(IContextListener* listener);
		void addKeyEventListener(IContextListener* listener);

		void addSigmaChangedListener(IContextListener* listener);
		void addDataChangedListener(IContextListener* listener);


	//CLASS DUMMY CONTEXT LISTENER

	//this class can be used by none objects to register a listener
	class DummyContextListener : public IContextListener{

		typedef void (*resizedFun)(int width, int height);
		typedef void (*mouseMovedFun)(int x, int y);
		typedef void (*mouseClickedFun)(int button, int action);
		typedef void (*mouseWheelFun)(int pos);
		typedef void (*keyEventFun)(int key, int action);
		typedef void (*sigmaChangedFun)( void );
		typedef void (*dataChangedFun)( void );

	public:
			DummyContextListener( void );

			//remember there exists no unregister at the moment so registering events can be called only once
			//multiple calls will cause undefined behaviour
			void activate(resizedFun resize, mouseMovedFun mouseMove, mouseClickedFun mouseClick,
								 keyEventFun keyEve, sigmaChangedFun sigmaChange, dataChangedFun dataChange,
								 mouseWheelFun mouseWheel);

		private:
			//implement the interface
			void resizeEvent(int width, int height);
		    void mouseMovedEvent(int x, int y);
			void mouseClickedEvent(int button, int action);
			void mouseWheelEvent(int pos);
			void keyEvent(int key, int action);
			void sigmaChanged( void );
			void dataChanged( void );

			resizedFun _resize;
			mouseMovedFun _mouseMove;
			mouseClickedFun _mouseClick;
			mouseWheelFun _mouseWheel;
			keyEventFun _keyEve;
			sigmaChangedFun _sigmaChange;
			dataChangedFun _dataChange;
	};
};


#endif