#ifndef EnvironmentHelper_H
#define EnvironmentHelper_H

#include <GL/glew.h>
#include <GL/glfw.h>
#include "GLSLShader.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <string>

	

typedef unsigned char byte;

typedef struct {
  byte red, green, blue;
} RGB_t;

namespace envHelper {

	GLSLShader* initShaders(vector<string> attributes, vector<string> uniforms, string vertShader, string fragShader, string gemShader="");

	GLuint loadRGBTexture(string filepath);
	void writeTexture2TGA(GLuint texID, string fileName);
	GLuint loadFloatGaussTextures(string filenamePrefix, int maxExp);
	void prepareEnvironment(int width, int height);
	void initGL( void );
	void cleanUp( void );

};

#endif