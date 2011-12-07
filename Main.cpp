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
void mouseWheelEvent(int pos);
void cleanUp( void );


//private vars
context::DummyContextListener _contextListener;


int main( int argc, const char* argv[] ) {
	//define an OpenGL context, open a window...
	envHelper::prepareEnvironment(WINDOW_WIDTH, WINDOW_HEIGHT);
	//set standart options
	envHelper::initGL();
	//init context and environment (remember a context needs a valid glfw environment before beeing ready)
	context::init();

	_contextListener.activate(0,0,0,keyEnv, mouseWheelEvent);

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
	context::cleanUp();
	envHelper::cleanUp();
}

void render( void ) {
	context::tick(); //process possible pending events

	//prepare
	glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	context::_renderer->work();
	context::_renderer->render();
	
	glfwSwapBuffers();
}


void keyEnv(int key, int action) {

	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_ESC) {
			context::_run = false;
		} else 
		if (key == '1') {
			context::setDataSet("_Data//LineNode.out");
		} else 
		if (key == '2') {
			context::setDataSet("_Data//GridNode.out");
		} else 
		if (key == '3') {
			context::setDataSet("_Data//WorkNode.out");
		} else 
		if (key == '4') {
			context::setDataSet("_Data//GermanyNode.out");
		} else 
		if (key == GLFW_KEY_KP_ADD) {
			context::setSideExponent(context::_sideExponent + 1);
		} else 
		if (key == GLFW_KEY_KP_SUBTRACT) {
			context::setSideExponent(context::_sideExponent - 1);
		}
	}
}

void mouseWheelEvent(int pos) {
	context::setZoomExponent(context::_zoomExponent + pos);
	glfwSetMouseWheel(0);
}