#include "LabelSelectionPainter.hpp"
#include <algorithm>

GLSLShader* LabelSelectionPainter::_l_shader_ptr = NULL;
GLSLShader* LabelSelectionPainter::_b_shader_ptr = NULL;

LabelSelectionPainter::LabelSelectionPainter( void ) {
	createShader();
	initVao();
	_renderer[4] = new TextRenderer("C://Windows//fonts//times.ttf", 28);
	_renderer[3] = new TextRenderer("C://Windows//fonts//times.ttf", 20);
	_renderer[2] = new TextRenderer("C://Windows//fonts//times.ttf", 16);
	_renderer[1] = new TextRenderer("C://Windows//fonts//times.ttf", 12);
	_renderer[0] = new TextRenderer("C://Windows//fonts//times.ttf", 10);
	_active = false;
}

LabelSelectionPainter::~LabelSelectionPainter( void ) {
	glDeleteBuffers(2, &_vbo[0]);
	glDeleteVertexArrays(2, &_vao[0]);
	for (int i = 0; i < 5; i++) {
		delete _renderer[i];
	}
}

void LabelSelectionPainter::cleanUp( void ) {
	delete _l_shader_ptr;
	delete _b_shader_ptr;
}


//public stuff

void LabelSelectionPainter::clear( void ) {
	_active = false;
}

void LabelSelectionPainter::setData(vector<int>* ids, const vector<Label>* indexedLabels) {
	_active = true;

	vector<Label> selected;
	for (int i = 0; i < ids->size(); i++) {
		selected.push_back(indexedLabels->at(ids->at(i)));
	}




	//update box
	float quad[8] = { -1.0f, 1.0f,     -1.0f,  0.0f,      -0.5f, 1.0f,     -0.5f,  0.0f};

	glBindBuffer(GL_ARRAY_BUFFER, _vbo[BOX]);
		glBufferData (GL_ARRAY_BUFFER, 8 * sizeof(float), &quad[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	//update lines
	float lines[4] = { -1.0f, -1.0f,     1.0f,  1.0f};

	glBindBuffer(GL_ARRAY_BUFFER, _vbo[LINES]);
		glBufferData (GL_ARRAY_BUFFER, 4 * sizeof(float), &lines[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	_lines = 1;


	delete ids;
}

void LabelSelectionPainter::renderSelection( void ) {
	if (_active) {	
		glBindVertexArray(_vao[BOX]);
			_b_shader_ptr->use();			
				glUniform4f(_b_shader_ptr->getUniformLocation("color"), 1.0f, 1.0f, 1.0f, 1.0f);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); 
			_b_shader_ptr->unUse();
		glBindVertexArray(0);

		glBindVertexArray(_vao[LINES]);
			_l_shader_ptr->use();			
				glUniform4f(_l_shader_ptr->getUniformLocation("color"), 0.0f, 0.0f, 0.0f, 0.0f);
				glDrawArrays(GL_LINES, 0, 2 * _lines); 
			_l_shader_ptr->unUse();
		glBindVertexArray(0);
	}
}


//private stuff

void LabelSelectionPainter::createShader( void ) {
	if (_l_shader_ptr == NULL) {
		vector<string> unis;
		vector<string> attribs;

		attribs.push_back("vVertex");
		unis.push_back("color");
		_l_shader_ptr = new GLSLShader(attribs, unis, "shaders/standard/shader.vert", "shaders/standard/shader.frag");
		_b_shader_ptr = new GLSLShader(attribs, unis, "shaders/standard/shader.vert", "shaders/standard/shader.frag");
	}
}

void LabelSelectionPainter::initVao( void ) {
	
	glGenVertexArrays(2, &_vao[0]);
	glGenBuffers (2, &_vbo[0]);

	glBindVertexArray(_vao[BOX]);	 
		glBindBuffer(GL_ARRAY_BUFFER, _vbo[BOX]);
			glEnableVertexAttribArray(_b_shader_ptr->getAttributeLocation("vVertex"));
			glVertexAttribPointer (_b_shader_ptr->getAttributeLocation("vVertex"), 2, GL_FLOAT, GL_FALSE, 0, 0);	
		glBindBuffer (GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glBindVertexArray(_vao[LINES]);	 
		glBindBuffer(GL_ARRAY_BUFFER, _vbo[LINES]);
			glEnableVertexAttribArray(_l_shader_ptr->getAttributeLocation("vVertex"));
			glVertexAttribPointer (_l_shader_ptr->getAttributeLocation("vVertex"), 2, GL_FLOAT, GL_FALSE, 0, 0);	
		glBindBuffer (GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}