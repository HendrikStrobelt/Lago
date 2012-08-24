#ifndef CONTEXT_H
#define CONTEXT_H

#include "IContextListener.hpp"
#include "ScaleOptions.hpp"
#include "../renderer/Renderer.hpp"
#include "Options.hpp"

#include <string>

using namespace std;

//context can only be used after calling init and after
//the glfw environment has been set up successfully
namespace context {

	void init();
	void cleanUp( void );

	//push/pull read and write no notification
		extern bool _run;
		extern float _pixelSize;
		extern Options _options;
		extern ScaleOptions _scaleOptions[];
		extern GLuint _textureTestHook;
	//pull
		extern Renderer* _renderer;	
		extern float _sideRatio;
		extern int _zoomExponent;
		extern int _sideExponent;
		extern float _zoomFactor;

		extern float _worldTransX;
		extern float _worldTransY;

		void tick( void );

		//setters
		void emptyClick( void );
		void rightClick(int x, int y);
		void labelClick(bool add, int id);
		void lockClick( void );

		void setSideExponent(int newSideExponent);
		void setZoomExponent(int newZoomExponent);
		void setNewData(string nodeFile, string edgeFile="");
		void visParameterChange( void );

		void updateWorldTranslate(int xMouseMove, int yMouseMove);

		//GLFW getters
		void getWindowSize(int* width, int* height);
		void getMousePos(int* x, int* y);
		int getMouseWheelValue( void );
		double getTime( void );

		//program getters
		float getQuadSidelength(float aspectSide);


		//register listeners
		void addResizeListener(IContextListener* listener);
		void addMouseMovedListener(IContextListener* listener);
		void addMouseClickedListener(IContextListener* listener);
		void addMouseWheelListener(IContextListener* listener);
		void addKeyEventListener(IContextListener* listener);

		void addSideRatioChangedListener(IContextListener* listener);
		void addDataChangedListener(IContextListener* listener);


	//CLASS DUMMY CONTEXT LISTENER

	//this class can be used by none objects to register a listener
	class DummyContextListener : public IContextListener{

		typedef void (*resizedFun)(int width, int height);
		typedef void (*mouseMovedFun)(int x, int y);
		typedef void (*mouseClickedFun)(int button, int action);
		typedef void (*mouseWheelFun)(int pos);
		typedef void (*keyEventFun)(int key, int action);

	public:
			DummyContextListener( void );

			//remember there exists no unregister at the moment so registering events can be called only once
			//multiple calls will cause undefined behaviour
			void activate(resizedFun resize, mouseMovedFun mouseMove, mouseClickedFun mouseClick,
								 keyEventFun keyEve, mouseWheelFun mouseWheel);

		private:
			//implement the interface
			void resizeEvent(int width, int height);
		    void mouseMovedEvent(int x, int y);
			void mouseClickedEvent(int button, int action);
			void mouseWheelEvent(int pos);
			void keyEvent(int key, int action);

			resizedFun _resize;
			mouseMovedFun _mouseMove;
			mouseClickedFun _mouseClick;
			mouseWheelFun _mouseWheel;
			keyEventFun _keyEve;
	};
};


#endif