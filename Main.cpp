#pragma comment(lib, "Ws2_32.lib")

//INCLUDES

//before all others
#include <winsock2.h>
#include <ws2tcpip.h>
#include <GL/glew.h>
#include <GL/glfw.h>
//end

#include <iostream>

//#include <vld.h>


#include "GlobalConstants.hpp"
#include "helper\EnvironmentHelper.hpp"
#include "context\Context.hpp"
#include "helper\MouseHandler.hpp"
#include "connection\Connection.hpp"

using namespace std;

//private methods
void render( void );
void keyEnv(int key, int action);
void cleanUp( void );


//private vars
context::DummyContextListener _contextListener;
Connection _syncConnection;

int main( int argc, const char* argv[] ) {
	//define an OpenGL context, open a window...
	envHelper::prepareEnvironment(WINDOW_WIDTH, WINDOW_HEIGHT);
	//set standart options
	envHelper::initGL();
	//init context and environment (remember a context needs a valid glfw environment before beeing ready)
	context::init();
	mouseHandler::init();

	_contextListener.activate(0,0,0,keyEnv, 0);

	glfwSetTime(0);	
	glfwSetMouseWheel(0);
	context::_run = true;

	while (context::_run) {
		render();
	}

	//Close program and terminate GLFW
	cleanUp();
	exit(EXIT_SUCCESS);
}

void cleanUp( void ) {
	mouseHandler::cleanUp();
	context::cleanUp();
	envHelper::cleanUp();
}

void render( void ) {
	context::tick(); //process possible pending events

	//prepare
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	context::_renderer->work();
	context::_renderer->render();

	static double lastSync;
	if (glfwGetTime() > lastSync + 0.25) {
		lastSync = glfwGetTime();
		_syncConnection.sync();
	}
	
	glfwSwapBuffers();
}


void keyEnv(int key, int action) {

	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_ESC) {
			context::_run = false;
		} else 
		if (key == '5') {
			GLuint tex, vao, vbo, fbo;
			glGenTextures(1, &tex);
			glGenVertexArrays(1, &vao);
			glGenBuffers(1, &vbo);
			glGenFramebuffers(1, &fbo);
				cout << "tex " << tex << " ,vao " << vao << " ,vbo " << vbo << " ,fbo " << fbo << "\n";
			glDeleteTextures(1, &tex);
			glDeleteVertexArrays(1, &vao);
			glDeleteBuffers(1, &vbo);
			glDeleteFramebuffers(1, &fbo);
		} else 
		if (key == GLFW_KEY_KP_ADD) {
			context::setSideExponent(context::_sideExponent + 1);
		} else 
		if (key == GLFW_KEY_KP_SUBTRACT) {
			context::setSideExponent(context::_sideExponent - 1);
		} else
		if (key == GLFW_KEY_F1) {
			//reconnect to gui if necessary
			_syncConnection.connect();
		}

	}
}
