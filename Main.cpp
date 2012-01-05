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

#include <vld.h>


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
context::DummyContextListener _contextListener;
Connection _syncConnection;

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
			} else if (parameter[0] == "EDGE_HIERARCHY_FLAT") {
				EDGE_HIERARCHY_FLAT = atoi(parameter[1].c_str());
			} else if (parameter[0] == "MIN_COORD") {
				MIN_COORD = atoi(parameter[1].c_str());
			} else if (parameter[0] == "MAX_COORD") {
				MAX_COORD = atoi(parameter[1].c_str());
			} else if (parameter[0] == "GAUSS_TEXTURE_PI") {
				GAUSS_TEXTURE_PI = atof(parameter[1].c_str());
			} else if (parameter[0] == "POINT_INIT_STEP") {
				POINT_INIT_STEP = atoi(parameter[1].c_str());
			} else if (parameter[0] == "PARTS_INIT_STEP") {
				PARTS_INIT_STEP = atoi(parameter[1].c_str());
			} else if (parameter[0] == "TE_KERNEL_SIZE") {
				TE_KERNEL_SIZE = atoi(parameter[1].c_str());
			} else if (parameter[0] == "TE_MAX_ITERATIONS") {
				TE_MAX_ITERATIONS = atoi(parameter[1].c_str());
			} else if (parameter[0] == "ZOOM_BASE") {
				ZOOM_BASE = atof(parameter[1].c_str());
			} else if (parameter[0] == "SIDE_BASE") {
				SIDE_BASE = atof(parameter[1].c_str());
			} else if (parameter[0] == "ANGLE_STEP") {
				ANGLE_STEP = atoi(parameter[1].c_str());
			} else if (parameter[0] == "OFF_ZOOM") {
				OFF_ZOOM = atoi(parameter[1].c_str());
			} else if (parameter[0] == "OFF_SHRINK") {
				OFF_SHRINK = atoi(parameter[1].c_str());
			} else if (parameter[0] == "START_MAX_LABELS") {
				START_MAX_LABELS = atoi(parameter[1].c_str());
			} else if (parameter[0] == "ORDER_LABELS") {
				ORDER_LABELS = atoi(parameter[1].c_str());
			} else if (parameter[0] == "OL_START") {
				OL_START = parameter[1];
			} else if (parameter[0] == "OL_PARAM") {
				OL_PARAM = parameter[1];
			} else if (parameter[0] == "OL_BOX_IDENTIFIER_START") {
				OL_BOX_IDENTIFIER_START = parameter[1];
			} else if (parameter[0] == "OL_BOX_IDENTIFIER_END") {
				OL_BOX_IDENTIFIER_END = parameter[1];
			} else if (parameter[0] == "OL_BOX_CENTER_START") {
				OL_BOX_CENTER_START = parameter[1];
			} else if (parameter[0] == "OL_BOX_CENTER_DELIM") {
				OL_BOX_CENTER_DELIM = parameter[1];
			} else if (parameter[0] == "OL_BOX_CENTER_END") {
				OL_BOX_CENTER_END = parameter[1];
			} else if (parameter[0] == "OL_BOX_WIDTH_START") {
				OL_BOX_WIDTH_START = parameter[1];
			} else if (parameter[0] == "OL_BOX_WIDTH_END") {
				OL_BOX_WIDTH_END = parameter[1];
			} else if (parameter[0] == "OL_BOX_HEIGHT_START") {
				OL_BOX_HEIGHT_START = parameter[1];
			} else if (parameter[0] == "OL_BOX_HEIGHT_END") {
				OL_BOX_HEIGHT_END = parameter[1];
			} else if (parameter[0] == "OL_ADDITIONAL_BOX_PARAMETERS") {
				OL_ADDITIONAL_BOX_PARAMETERS = parameter[1];
			} else if (parameter[0] == "OL_END") {
				OL_END = parameter[1];
			} else if (parameter[0] == "OL_RETURN_START") {
				OL_RETURN_START = parameter[1];
			} else if (parameter[0] == "ORDERING_TOOL_PATH") {
				ORDERING_TOOL_PATH = parameter[1];
			} else if (parameter[0] == "ORDERING_TOOL_PARAMETERS") {
				ORDERING_TOOL_PARAMETERS = parameter[1];
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