#pragma comment(lib, "Ws2_32.lib")

//INCLUDES

//before all others
#include <winsock2.h>
#include <ws2tcpip.h>
#include <GL/glew.h>
#include <GL/glfw.h>
//end

#include <iostream>
#include <fstream>

//#include <vld.h>

#include "renderer\Cinema.hpp"
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
void initGlobalConstants( void );
vector<string> split(string line);

//private vars
enum RENDER_MODE {GRAPH, DENSITY_FIELD, EVAL_FIELD, LINE_FIELD};
RENDER_MODE _mode;

Cinema* _cinPtr;
context::DummyContextListener _contextListener;
Connection _syncConnection;


#include "renderer\cinemaScripts\Europe.hpp"

int main( int argc, const char* argv[] ) {
	//load parameters
	initGlobalConstants();
	//define an OpenGL context, open a window...
	envHelper::prepareEnvironment(WINDOW_WIDTH, WINDOW_HEIGHT);
	//set standart options
	envHelper::initGL();
	//init context and environment (remember a context needs a valid glfw environment before beeing ready)
	context::init();
	mouseHandler::init();

	_contextListener.activate(0,0,0,keyEnv, 0);
	_mode = GRAPH;

	glfwSetTime(0);	
	glfwSetMouseWheel(0);
	context::_run = true;
	_cinPtr = new Cinema();
	EuropeScript::setScript(_cinPtr);

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
	delete _cinPtr;
}

void render( void ) {
	context::tick(); //process possible pending events

	//prepare
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	double before = glfwGetTime();
		context::_renderer->work();
	double workTime = glfwGetTime() - before;

	_cinPtr->run(workTime);

	switch(_mode) {
		case GRAPH:			context::_renderer->render();				break;
		case DENSITY_FIELD: context::_renderer->renderGauss();			break;
		case EVAL_FIELD:	context::_renderer->renderEvalField();		break;
		case LINE_FIELD:	context::_renderer->renderLineField();		break;
	}

	_cinPtr->rendered();

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
		if (key == GLFW_KEY_KP_ADD) {
			context::setSideExponent(context::_sideExponent + 1);
		} else 
		if (key == GLFW_KEY_KP_SUBTRACT) {
			context::setSideExponent(context::_sideExponent - 1);
		} else
		if (key == GLFW_KEY_F1) {
			//reconnect to gui if necessary
			_syncConnection.connect();
		} else 
		if (key == GLFW_KEY_F5) {
			_mode = GRAPH;
		} else 
		if (key == GLFW_KEY_F6) {
			_mode = DENSITY_FIELD; //alias gauss
		} else 
		if (key == GLFW_KEY_F7) {
			_mode = EVAL_FIELD;
		} else 
		if (key == GLFW_KEY_F8) {
			_mode = LINE_FIELD;
		}
	}
}


void initGlobalConstants( void ) {
    fstream f;
    char cstring[1024];
    f.open("HGvis.ini", ios::in);

    while (!f.eof())
    {
        f.getline(cstring, sizeof(cstring));
		vector<string> parameter = split(cstring);

		if (parameter.size() == 2) {
			if (parameter[0] == "WINDOW_WIDTH") {
				WINDOW_WIDTH = atoi(parameter[1].c_str());
			} else if (parameter[0] == "WINDOW_HEIGHT") {
				WINDOW_HEIGHT = atoi(parameter[1].c_str());
			} else if (parameter[0] == "NODE_FILE_5") {
				STANDARD_NODE = parameter[1].c_str();
			} else if (parameter[0] == "EDGE_FILE_5") {
				STANDARD_EDGE = parameter[1].c_str();
			}
		}
    }
    f.close();
}

vector<string> split(string line) {
	int split = line.find(" ");
	vector<string> ret;

	if (split != string::npos) {
		ret.push_back(line.substr(0, split));
		
		string param = line.substr(split+1, line.size() - split -1);
		char* cstr = new char [param.size()+1];
		char* cstr2 = new char [param.size()+1];
		strcpy (cstr, param.c_str());

		int j = 0;
		bool skip = false;
		bool skip2 = false;
		for (int i = 0; i < (param.size()); i++) {
			if (!skip) {
				if (cstr[i] == '"') {
					j--;
				} else 
				if (cstr[i] == '\\' && cstr[i+1] == '\\') {
					cstr2[j] = '\\';
					j++;
					cstr2[j] = '\\';
					j++;
					cstr2[j] = cstr[i+2];
					skip = true;
					skip2 = true;
				} else
				if (cstr[i] == '\\' && cstr[i+1] == 'n') {
					cstr2[j] = '\n';
					skip = true;
				} else
				if (cstr[i] == '\\' && cstr[i+1] == '"') {
					cstr2[j] = '\"';
					skip = true;
				} else {
					cstr2[j] = cstr[i];
				}
				j++;
			} else {
				if (!skip2) {
					skip = false;
				}
				skip2 = false;
			}
		}

		cstr2[j] = '\0';
		ret.push_back(cstr2);
		delete cstr;
		delete cstr2;
	}

	return ret;
}