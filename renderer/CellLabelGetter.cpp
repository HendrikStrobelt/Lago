#include "CellLabelGetter.hpp"
#include "../Node.hpp"
#include "../context/Context.hpp"
#include <glm\gtc\type_ptr.hpp>

GLSLShader* CellLabelGetter::_shader_ptr = NULL;
GLSLShader* CellLabelGetter::_eval_shader_ptr = NULL;

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
	glDeleteBuffers(2, &_evalVBO[0]);
	glDeleteVertexArrays(1, &_evalVAO);
}

void CellLabelGetter::cleanUp() {
	delete _shader_ptr;
	delete _eval_shader_ptr;
}


vector<int>* CellLabelGetter::getLabelIndices(int mouseX, int mouseY, GLuint fieldTex, GLuint gaussTex, glm::mat4 MVP) {
	int w,h;
	context::getWindowSize(&w, &h);
	float texX = (float)mouseX / (float)w;
	float texY = (float)(h - mouseY) / (float) h;

	//upgrade eval field
	FrameBufferContainer* evalFBC = new FrameBufferContainer(w,h);
	glBindFramebuffer(GL_FRAMEBUFFER, evalFBC->_fbo);
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fieldTex);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, gaussTex);
				glBindVertexArray(_evalVAO);
					_eval_shader_ptr->use();			
						glUniform1i(_eval_shader_ptr->getUniformLocation("evalTexture"), 0);
						glUniform1i(_eval_shader_ptr->getUniformLocation("screenTexture"), 1);	
						glUniform2f(_eval_shader_ptr->getUniformLocation("compareTexCoord"), texX, texY);
						glUniform1i(_eval_shader_ptr->getUniformLocation("width"), w);
						glUniform1i(_eval_shader_ptr->getUniformLocation("height"), h);
						glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); 
					_eval_shader_ptr->unUse();
				glBindVertexArray(0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D,  0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,  0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	GLuint evalField = evalFBC->detachTexture();
	delete evalFBC;
	//test againt mouse

	glEnable(GL_RASTERIZER_DISCARD);		
		glBindTexture(GL_TEXTURE_2D, evalField);
				glBindVertexArray(_dataVAO);
					_shader_ptr->use();		
						glUniformMatrix4fv(_shader_ptr->getUniformLocation("MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
						glUniform1i(_shader_ptr->getUniformLocation("fieldTex"), 0);
						glUniform2f(_shader_ptr->getUniformLocation("compareTexCoord"), texX, texY);
						glUniform1i(_shader_ptr->getUniformLocation("width"), w);
						glUniform1i(_shader_ptr->getUniformLocation("height"), h);

							glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, _capturedVBO); 
							glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, _query); 
								glBeginTransformFeedback(GL_POINTS);
									glDrawArrays(GL_POINTS, 0, _size); 
								glEndTransformFeedback();
							glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN); 			
								
					_shader_ptr->unUse();
				glBindVertexArray(0);		
		glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_RASTERIZER_DISCARD);
		
	GLuint count = 0;
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

	glDeleteTextures(1, &evalField);

	return ret;
}


// private stuff


void CellLabelGetter::createShader( void ) {
	if (_shader_ptr == NULL) {
		vector<string> unis;
		vector<string> attribs;
		vector<string> transFeed;
		
		transFeed.push_back("labelID");

		attribs.push_back("vVertex");
		attribs.push_back("vLabelID");
		unis.push_back("MVP");
		unis.push_back("fieldTex");
		unis.push_back("gaussTex");
		unis.push_back("compareTexCoord");
		unis.push_back("width");
		unis.push_back("height");

		_shader_ptr = new GLSLShader(transFeed, attribs, unis, "shaders/cellLabel/cellLabelGetter.vert", "shaders/cellLabel/cellLabelGetter.frag", "shaders/cellLabel/cellLabelGetter.gem");

		unis.clear();
		attribs.clear();

		attribs.push_back("vVertex");
		attribs.push_back("vTex");

		unis.push_back("evalTexture");
		unis.push_back("screenTexture");
		unis.push_back("compareTexCoord");
		unis.push_back("width");
		unis.push_back("height");

		_eval_shader_ptr = new GLSLShader(attribs, unis, "shaders/cellLabel/mouseEvaluator.vert","shaders/cellLabel/mouseEvaluator.frag");
	}
}

void CellLabelGetter::initVao(GLuint vbo) {

	glGenVertexArrays(1, &_dataVAO);
	glGenBuffers(1, &_capturedVBO);

	//Data
	glBindVertexArray(_dataVAO);	 
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glEnableVertexAttribArray(_shader_ptr->getAttributeLocation("vVertex"));
			glVertexAttribPointer (_shader_ptr->getAttributeLocation("vVertex"), 2, GL_FLOAT, GL_FALSE, sizeof(PackedNode), 0);	
			glEnableVertexAttribArray(_shader_ptr->getAttributeLocation("vLabelID"));
			glVertexAttribIPointer(_shader_ptr->getAttributeLocation("vLabelID"), 1, GL_INT, sizeof(PackedNode), (void*)(2 * sizeof(float) + sizeof(short)));
	glBindVertexArray(0);


	//captured
	glBindBuffer(GL_ARRAY_BUFFER, _capturedVBO);
		glBufferData (GL_ARRAY_BUFFER, _size * sizeof(int), NULL, GL_DYNAMIC_READ);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	//mouse eval

	float quad[8] = { -1.0f, 1.0f,    -1.0f, -1.0f,		 1.0f, 1.0f, 	 1.0f, -1.0f};
	float texture[8] = { 0.0f, 1.0f,     0.0f,  0.0f,      1.0f, 1.0f,     1.0f,  0.0f};

	glGenVertexArrays(1, &_evalVAO);
	glGenBuffers (2, &_evalVBO[0]);


	glBindBuffer(GL_ARRAY_BUFFER, _evalVBO[TEX]);
		glBufferData (GL_ARRAY_BUFFER, 8 * sizeof(float), &texture[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, _evalVBO[VERTEX]);
		glBufferData (GL_ARRAY_BUFFER, 8 * sizeof(float), &quad[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindVertexArray(_evalVAO);	 
		glBindBuffer(GL_ARRAY_BUFFER, _evalVBO[VERTEX]);
			glEnableVertexAttribArray(_shader_ptr->getAttributeLocation("vVertex"));
			glVertexAttribPointer (_shader_ptr->getAttributeLocation("vVertex"), 2, GL_FLOAT, GL_FALSE, 0, 0);	
		glBindBuffer(GL_ARRAY_BUFFER, _evalVBO[TEX]);
			glEnableVertexAttribArray(_shader_ptr->getAttributeLocation("vTex"));
			glVertexAttribPointer (_shader_ptr->getAttributeLocation("vTex"), 2, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer (GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}