//before all others
#include <winsock2.h>
#include <ws2tcpip.h>
#include <GL/glew.h>
#include <GL/glfw.h>
//end


#include <vld.h>


#include "GlobalConstants.hpp"
#include "helper\EnvironmentHelper.hpp"
#include "context\Context.hpp"


//private methods
void render( void );
void keyEnv(int key, int action);
void cleanUp( void );


//private vars
context::DummyContextListener _contextListener;


int main( int argc, const char* argv[] ) {
	//define an OpenGL context, open a window...
	envHelper::prepareEnvironment(WINDOW_WIDTH, WINDOW_HEIGHT);
	//init context and environment (remember a context needs a valid glfw environment before beeing ready)
	context::init();
	//set standart options
	envHelper::initGL();

	_contextListener.activate(0,0,0,keyEnv,0, 0,0);

	glfwSetTime(0);	
	while (context::_run) {
		render();
	}

	//Close program and terminate GLFW
	cleanUp();
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void cleanUp( void ) {
	context::cleanUp();
}

void render( void ) {
	glfwSwapBuffers();
}


void keyEnv(int key, int action) {
	if (key == GLFW_KEY_ESC) {
		context::_run = false;
	}
}