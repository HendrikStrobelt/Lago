#ifndef EnvironmentHelper_H
#define EnvironmentHelper_H

#include <GL/glew.h>
#include <GL/glfw.h>
#include "GLSLShader.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <string>

namespace envHelper {

	GLSLShader* initShaders(vector<string> attributes, vector<string> uniforms, string vertShader, string fragShader, string gemShader="");

	GLuint loadRGBTexture(string filepath);
	GLuint loadSmoothRGBTexture(string filepath);
	GLuint loadFloatGaussTextures(string filenamePrefix, int maxExp);
	void prepareEnvironment(int width, int height);
	void initGL( void );
	void cleanUp( void );

};

#endif