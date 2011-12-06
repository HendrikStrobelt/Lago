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
#include "renderer\Renderer.hpp"

//private methods
void render( void );
void keyEnv(int key, int action);
void cleanUp( void );


//private vars
context::DummyContextListener _contextListener;
Renderer* _renderer;

int main( int argc, const char* argv[] ) {
	//define an OpenGL context, open a window...
	envHelper::prepareEnvironment(WINDOW_WIDTH, WINDOW_HEIGHT);
	//init context and environment (remember a context needs a valid glfw environment before beeing ready)
	context::init();
	//set standart options
	envHelper::initGL();

	_contextListener.activate(0,0,0,keyEnv,0, 0,0);
	_renderer = new Renderer();


	glfwSetTime(0);	
	while (context::_run) {
		render();
	}

	//Close program and terminate GLFW
	cleanUp();
	exit(EXIT_SUCCESS);
}

void cleanUp( void ) {
	delete _renderer;
	context::cleanUp();
	envHelper::cleanUp();
}

void render( void ) {
	//prepare
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	_renderer->work();
	_renderer->render();
	
	glfwSwapBuffers();
}


void keyEnv(int key, int action) {
	if (key == GLFW_KEY_ESC) {
		context::_run = false;
	}
}