#include "EnvironmentHelper.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <sstream>

//painters needed to free static shaders
#include "../painter/ProgressbarPainter.hpp"
#include "../painter/GaussPainter.hpp"
#include "../painter/DisplayConvertPainter.hpp"
#include "../painter/VisPainter.hpp"
#include "../painter/DividedLinePainter.hpp"
#include "../painter/ScalingPainter.hpp"
#include "../renderer/TextureExaminer.hpp"
#include "../renderer/FieldEvaluation.hpp"
#include "../renderer/CellLabelGetter.hpp"

#include "../context/Context.hpp"

////////////////////////Variables////////////////////////////

namespace envHelper {

	//private vars
	context::DummyContextListener _contextListener;

	//private methods
	void resize(int width, int height);
	void deleteStaticShaders( void );
	//public

	void cleanUp( void ) {
		deleteStaticShaders();
		glfwTerminate();
	}


	GLuint loadRGBTexture(string filepath) {
		GLuint handle;

		glGenTextures(1, &handle);
		glBindTexture(GL_TEXTURE_2D, handle);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glfwLoadTexture2D(filepath.c_str(), 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		return handle;
	}


	GLuint loadFloatGaussTextures(string filenamePrefix, int maxExp) {
		vector<float *> dataVec;

		for (int i = 0; i <= maxExp; i++) {
			FILE* file_ptr;
			int size = (int) pow(2.0f,i);
			string filename = filenamePrefix;
	
			stringstream ss;
			ss << i;

			filename += ss.str();
			filename += ".ftd";

			float* textData = new float[size*size];

			if( (fopen_s(&file_ptr, filename.data(), "rb")) != 0) {
				cout << "file opening failed" << "\n";	
			} else {
				fread(textData,  sizeof(float), (size*size), file_ptr);
				fclose(file_ptr); // Closes the file stream
			}


			dataVec.push_back(textData);
		}

		GLuint tex;
		glGenTextures(1, &tex);

		glBindTexture(GL_TEXTURE_2D, tex);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			for (int i = maxExp; i >= 0; i--) {
				int size = (int) pow(2.0f,i);
				glTexImage2D(GL_TEXTURE_2D, maxExp-i , GL_RGBA32F, size, size, 0, GL_RED, GL_FLOAT, dataVec[i]);
			}
		glBindTexture(GL_TEXTURE_2D, 0);
		
		for (int i = 0; i <= maxExp; i++) {
			delete[] dataVec[i];
		}

		return tex;
	}



	//activate opengl stuff
	void initGL( void ) {
		glClearColor(0.0f,0.0f,0.0f,0.0f);	
		glEnable(GL_BLEND);	
	}


	//initialize GLFW and GLEW writes errors to std::cerr
	void prepareEnvironment(int width, int height) {
		//Initialize GLFW
		if (!glfwInit()) {
			std::cerr << "Error: initializing GLFW failed";
			exit(EXIT_FAILURE);
		}

		//set up additional parameters for the window
		glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR , 3);
		glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR , 3);

		// Open an OpenGL window
		if (!glfwOpenWindow(width, height, 0, 0, 0, 0, 0, 0, GLFW_WINDOW)) {
			std::cerr << "Error: opening of the GLFW window fialed";
			glfwTerminate();
			exit(EXIT_FAILURE);
		}

		cout << "created OpenGL Context v";
		cout << glfwGetWindowParam(GLFW_OPENGL_VERSION_MAJOR) << ".";
		cout << glfwGetWindowParam(GLFW_OPENGL_VERSION_MINOR) << "\n";

		//program context is ready now (init call in main and glfw context creation above)
		//register resize listener
		_contextListener.activate(resize,0,0,0,0);

		//Initialize GLEW
		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		
		if (GLEW_OK != err)	{
			std::cerr << "Error: " << glewGetErrorString(err) << "\n";
			glfwTerminate();
			exit(EXIT_FAILURE);
		}
	}


	//private
	void resize(int width, int height) {
		glViewport(0,0, (GLsizei) width, (GLsizei) height);
	}

	void deleteStaticShaders( void ) {
		ProgressbarPainter::cleanUp();
		GaussPainter::cleanUp();
		DisplayConvertPainter::cleanUp();
		VisPainter::cleanUp();
		DividedLinePainter::cleanUp();
		ScalingPainter::cleanUp();

		TextureExaminer::cleanUp();
		FieldEvaluation::cleanUp();
		CellLabelGetter::cleanUp();
	}
};