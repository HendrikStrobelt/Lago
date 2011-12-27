#include "GLSLShader.hpp"
#include <fstream>
#include <iostream>


GLSLShader::GLSLShader(string transformFeedbackVar, vector<string> attributes, vector<string> uniforms, string vertShader, string fragShader, string gemShader) {
	_transformFeedback = true;
	_transformFeedbackVar = transformFeedbackVar;
	GLSLShader(attributes, uniforms, vertShader, fragShader, gemShader);
}
 
GLSLShader::GLSLShader(vector<string> attributes, vector<string> uniforms, string vertShader, string fragShader, string gemShader) {
	_shaders[VERTEX_SHADER] = 0;
	_shaders[FRAGMENT_SHADER] = 0;
	_shaders[GEOMETRY_SHADER] = 0;
	_transformFeedback = false;

	load(GL_VERTEX_SHADER, vertShader);
	load(GL_FRAGMENT_SHADER, fragShader);
	if (!gemShader.empty()) {
		load(GL_GEOMETRY_SHADER, gemShader);
	}

	createAndLinkProgram();
		
	use();	
		for (unsigned int i = 0; i < attributes.size(); i++) {
			_attributeMap[attributes[i]] = glGetAttribLocation(_program, attributes[i].c_str());	
		}
		for (unsigned int i = 0; i < uniforms.size(); i++) {
			_uniformMap[uniforms[i]] = glGetUniformLocation(_program, uniforms[i].c_str());
		}
	unUse();
}

GLSLShader::~GLSLShader(void) {
	glDeleteProgram(_program);
}


//public

void GLSLShader::use( void ) {
	glUseProgram(_program);
}

void GLSLShader::unUse( void ) {
	glUseProgram(0);
}

GLuint GLSLShader::getAttributeLocation(const string attribute) {
	return _attributeMap[attribute];
}

GLuint GLSLShader::getUniformLocation(const string uniform){
	return _uniformMap[uniform];
}


//private creation stuff

void GLSLShader::createAndLinkProgram( void ) {
	_program = glCreateProgram ();
	
	glAttachShader (_program, _shaders[VERTEX_SHADER]);
	glAttachShader (_program, _shaders[FRAGMENT_SHADER]);
	if (_shaders[GEOMETRY_SHADER] != 0) {
		glAttachShader (_program, _shaders[GEOMETRY_SHADER]);
	}
	
	//add output variable for color
	glBindFragDataLocation(_program, 0, "fragColor");

	if (_transformFeedback) {
		//add output for transform feedback
		GLchar const * outs[1];
		outs[0] = _transformFeedbackVar.c_str();
		glTransformFeedbackVaryings(_program, 1, outs, GL_SEPARATE_ATTRIBS); 
	}

	//link and check whether the program links fine
	GLint status;
	
	glLinkProgram(_program);
	glGetProgramiv(_program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		GLint logLength;
		glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &logLength);
		GLchar *infoLog= new GLchar[logLength];
		
		glGetProgramInfoLog (_program, logLength, NULL, infoLog);
		
		cerr << "Link log: " << infoLog << endl;
		delete[] infoLog;
	}

	glDeleteShader(_shaders[VERTEX_SHADER]);
	glDeleteShader(_shaders[FRAGMENT_SHADER]);
	glDeleteShader(_shaders[GEOMETRY_SHADER]);
}


void GLSLShader::load(GLenum whichShader, const string filename){
	ifstream in;
	in.open(filename.c_str(), ios_base::in);
	
	if(in) {		 	
		string line, shaderText;
		while(getline(in, line)) {
			shaderText.append(line);
			shaderText.append("\r\n");
		}		 

		GLuint shader = glCreateShader(whichShader);
		const char* shaderCstr = shaderText.c_str();
		glShaderSource(shader, 1, &shaderCstr, NULL);
		glCompileShader(shader);

		GLint status;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	
		if (status == GL_FALSE) {
			GLint logLength;		
			glGetShaderiv (shader, GL_INFO_LOG_LENGTH, &logLength);
			GLchar* infoLog= new GLchar[logLength];
			glGetShaderInfoLog(shader, logLength, NULL, infoLog);
		
			cerr << "Compile log: " << infoLog << "\n";
			delete [] infoLog;
		}

		switch(whichShader) {
			case GL_VERTEX_SHADER:
				_shaders[VERTEX_SHADER] = shader;
			break;
			case GL_FRAGMENT_SHADER:
				_shaders[FRAGMENT_SHADER] = shader;
			break;			
			case GL_GEOMETRY_SHADER:
				_shaders[GEOMETRY_SHADER] = shader;
			break;
		}

	} else {
		cerr << "Error loading shader: " << filename << "\n";
	}
}