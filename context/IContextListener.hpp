#ifndef ICONTEXTLISTENER_H
#define ICONTEXTLISTENER_H

class IContextListener
{
    public:
        virtual ~IContextListener() {}; //important to include a virtual destructor
								     	//other classes that hold a reference to the interface
									    //have to delete the "object" not the interface
        
		//interface methods
		
		//GLFW handlers
		virtual void resizeEvent(int width, int height) {};
		virtual void mouseMovedEvent(int x, int y) {};
		virtual void mouseClickedEvent(int button, int action) {};
		virtual void mouseWheelEvent(int pos) {};
		virtual void keyEvent(int key, int action) {};

		//program handlers
		virtual void sigmaChanged( void ) {};
		virtual void dataChanged( void ) {};
};


#endif