#include "CellLabelGetter.hpp"
#include "../Node.hpp"
#include "../context/Context.hpp"
#include <glm\gtc\type_ptr.hpp>

GLSLShader* CellLabelGetter::_shader_ptr = NULL;

CellLabelGetter::CellLabelGetter(GLuint nodeVbo, int size) {
	_size = size;
	createShader();
	initVao(nodeVbo);
	glGenQueries(1, &_query);
}

CellLabelGetter::~CellLabelGetter( void ) {
	glDeleteQueries(1, &_query);
	glDeleteBuffers(1, &_capturedVBO);
	glDeleteVertexArrays(1, &_dataVAO);
}

void CellLabelGetter::cleanUp() {
	delete _shader_ptr;
}


vector<int>* CellLabelGetter::getLabelIndices(int mouseX, int mouseY, GLuint fieldTex, glm::mat4 MVP) {
	int w,h;
	context::getWindowSize(&w, &h);
	float texX = (float)mouseX / (float)w;
	float texY = (float)(h - mouseY) / (float) h;


//	glEnable(GL_RASTERIZER_DISCARD);		
		glBindVertexArray(_dataVAO);
			_shader_ptr->use();		
				glUniformMatrix4fv(_shader_ptr->getUniformLocation("MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
				glUniform1i(_shader_ptr->getUniformLocation("fieldTex"), fieldTex);
				glUniform2f(_shader_ptr->getUniformLocation("compareTexCoord"), texX, texY);

					glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, _capturedVBO); 
					glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, _query); 
						glBeginTransformFeedback(GL_POINTS);
							glDrawArrays(GL_POINTS, 0, 4); 
						glEndTransformFeedback();
					glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN); 			
								
			_shader_ptr->unUse();
		glBindVertexArray(0);		
//	glDisable(GL_RASTERIZER_DISCARD);
		
/*	GLuint count = 0;
	glGetQueryObjectuiv(_query, GL_QUERY_RESULT, &count);

	//map buffer
	vector<int>* ret = new vector<int>;
	ret->resize(count);

	if (count > 0) {
		glBindBuffer(GL_ARRAY_BUFFER, _capturedVBO);
			GLvoid* vbo_buffer = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
				memcpy(&ret->at(0), vbo_buffer, count * sizeof(int));
			glUnmapBuffer(GL_ARRAY_BUFFER); 
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	return ret;*/ return NULL;
}


// private stuff


void CellLabelGetter::createShader( void ) {
	if (_shader_ptr == NULL) {
		vector<string> unis;
		vector<string> attribs;

		attribs.push_back("vVertex");
		//attribs.push_back("vLabelID");
		unis.push_back("MVP");
		unis.push_back("fieldTex");
		unis.push_back("compareTexCoord");

		_shader_ptr = new GLSLShader("labelID", attribs, unis, "shaders/cellLabel/cellLabelGetter.vert", "shaders/cellLabel/cellLabelGetter.frag");
	}
}

void CellLabelGetter::initVao(GLuint vbo) {

	glGenVertexArrays(1, &_dataVAO);
	glGenBuffers(1, &_capturedVBO);

	float quad[8] = { 0.0f, 1.0f,     0.0f,  0.0f,      1.0f, 1.0f,     1.0f,  0.0f};

	glGenBuffers (1, &_vbo);


	//Data
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData (GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), &quad[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Data
	glBindVertexArray(_dataVAO);	 
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
			glEnableVertexAttribArray(_shader_ptr->getAttributeLocation("vVertex"));
			glVertexAttribPointer (_shader_ptr->getAttributeLocation("vVertex"), 2, GL_FLOAT, GL_FALSE, /* sizeof(PackedNode) */0, 0);	
/*			glEnableVertexAttribArray(_shader_ptr->getAttributeLocation("vLabelID"));
			glVertexAttribIPointer(_shader_ptr->getAttributeLocation("vLabelID"), 1, GL_INT, sizeof(PackedNode), (void*)(2 * sizeof(float) + sizeof(short)));*/
	glBindVertexArray(0);


	//captured
	glBindBuffer(GL_ARRAY_BUFFER, _capturedVBO);
		glBufferData (GL_ARRAY_BUFFER, _size * sizeof(int), NULL, GL_DYNAMIC_READ);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}