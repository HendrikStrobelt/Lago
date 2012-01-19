#ifndef GLSLShader_H
#define GLSLShader_H

#include <GL/glew.h>
#include <GL/glfw.h>
#include <map>
#include <string>
#include <vector>

using namespace std;


class GLSLShader {
	public:
		GLSLShader(vector<string> attributes, vector<string> uniforms, string vertShader, string fragShader, string gemShader="");
		GLSLShader(vector<string> transformFeedbackVars, vector<string> attributes, vector<string> uniforms, string vertShader, string fragShader, string gemShader="");
		~GLSLShader(void);	
	
		void use( void );
		void unUse( void );

		GLuint getAttributeLocation(const string attribute);
		GLuint getUniformLocation(const string uniform);

	private:
		void load(GLenum whichShader, const string filename);
		void createAndLinkProgram( void );

		enum ShaderType {VERTEX_SHADER, FRAGMENT_SHADER, GEOMETRY_SHADER};
		bool _transformFeedback;
		vector<string> _transformFeedbackVars;
		GLuint	_program;
		GLuint _shaders[3];
		map<string,GLuint> _attributeMap;
		map<string,GLuint> _uniformMap;
};	


#endif