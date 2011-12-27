#ifndef CELL_LABEL_GETTER_H
#define CELL_LABEL_GETTER_H

#include <GL\glew.h>
#include <GL\glfw.h>
#include <glm\glm.hpp>
#include <vector>
#include "../helper/GLSLShader.hpp"

using namespace std;

class CellLabelGetter {

	public:
		CellLabelGetter(GLuint nodeVbo, int size);
		~CellLabelGetter( void );

		vector<int>* getLabelIndices(int mouseX, int mouseY, GLuint fieldTex, glm::mat4 MVP);

		//static clean up
		static void cleanUp( void );
	private:

		static void createShader( void );
		static GLSLShader* _shader_ptr;

		void initVao(GLuint vbo);
		
		int _size;
		GLuint _query;
		GLuint _dataVAO;
		GLuint _capturedVBO;

				GLuint _vbo;

};

#endif