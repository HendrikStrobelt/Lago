#include "LabelSelectionPainter.hpp"

#include <algorithm>
#include <sstream>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "../context/Context.hpp"
#include "../text/PreparedText.hpp"
#include "../helper/MouseHandler.hpp"


GLSLShader* LabelSelectionPainter::_l_shader_ptr = NULL;
GLSLShader* LabelSelectionPainter::_b_shader_ptr = NULL;
GLSLShader* LabelSelectionPainter::_c_shader_ptr = NULL;

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
	glDeleteBuffers(4, &_vbo[0]);
	glDeleteVertexArrays(3, &_vao[0]);
	for (int i = 0; i < 5; i++) {
		delete _renderer[i];
	}
}

void LabelSelectionPainter::cleanUp( void ) {
	delete _l_shader_ptr;
	delete _b_shader_ptr;
	delete _c_shader_ptr;
}


//public stuff

void LabelSelectionPainter::resize(int w, int h) {
	for (int i = 0; i < 5; i++) {
		_renderer[i]->clearTextStorage();
		_renderer[i]->resize(w, h);
	}
}

void LabelSelectionPainter::clear( void ) {
	_active = false;
}

void LabelSelectionPainter::changePanning(int xShift, int yShift) {
	int w,h;
	context::getWindowSize(&w, &h);
	
	_clickX += (float)xShift / (float)w;
	_clickY -= (float)yShift / (float)h;
}

void LabelSelectionPainter::setData(vector<int>* ids, const vector<Label>* indexedLabels, int mouseX, int mouseY) {
	if (ids->size() > 0) {
		int w,h;
		context::getWindowSize(&w, &h);

		//mouse update in 0..1 0/0 left down
			_clickX = mouseX / (float)w;
			_clickY = 1.0f - mouseY / (float)h;
		//

		for (int i = 0; i < 5; i++) {
			_renderer[i]->clearTextStorage();
		} 

		_active = true;
	
		vector<Label> selected;
		for (int i = 0; i < ids->size(); i++) {
			selected.push_back(indexedLabels->at(ids->at(i)));
		}
		
		int xSize, ySize;
		vector<float> yAnchor;
		sortLabels(&selected);
		prepareTextRenderer(&xSize, &ySize, &yAnchor, &selected);

		//update box
		float quad[8] = { -1.0f, 1.0f,
						  -1.0f,  1.0f - ((float)ySize / (float) h * 2.0f),
						  -1.0f + ((float)xSize / (float) w * 2.0f), 1.0f,
						  -1.0f + ((float)xSize / (float) w * 2.0f), 1.0f - ((float)ySize / (float) h * 2.0f)};

		glBindBuffer(GL_ARRAY_BUFFER, _vbo[BOX]);
			glBufferData (GL_ARRAY_BUFFER, 8 * sizeof(float), &quad[0], GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	
		//update lines
		float xAnchor = -1.0f + ((float)xSize / (float) w * 2.0f);
		float* lines = new float[yAnchor.size() * 4];
		
		for (int i = 0; i < yAnchor.size(); i++) {
			lines[i * 4] = xAnchor;
			lines[i * 4 + 1] = yAnchor[i];
			lines[i * 4 + 2] = selected[i].x;
			lines[i * 4 + 3] = selected[i].y;
		}

		glBindBuffer(GL_ARRAY_BUFFER, _vbo[LINES]);
			glBufferData (GL_ARRAY_BUFFER, 4 * yAnchor.size() * sizeof(float), &lines[0], GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		_lines = yAnchor.size();

		delete lines;
	} else {
		_active = false;
	}

	delete ids;
}

void LabelSelectionPainter::renderSelection(glm::mat4 MVP, GLuint evalTex, int xShift, int yShift, int xMove, int yMove) {
	if (_active) {
		int w,h;
		context::getWindowSize(&w, &h);

		//lines
		glm::mat4 MVP_S = glm::translate(glm::mat4(1.0f), glm::vec3((float)xShift / (float)w * 2.0f, -(float)yShift / (float)h * 2.0f, 0.0f));
		MVP_S = MVP_S * MVP;

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glLineWidth(2.0f);
		glBindVertexArray(_vao[LINES]);
			glBindTexture(GL_TEXTURE_2D, context::_options._labelScheme);
				_l_shader_ptr->use();			
					glUniform1i(_l_shader_ptr->getUniformLocation("colorScheme"), 0);
					glUniformMatrix4fv(_l_shader_ptr->getUniformLocation("MVP"), 1, GL_FALSE, glm::value_ptr(MVP_S));
					glDrawArrays(GL_POINTS, 0, _lines); 
				_l_shader_ptr->unUse();
			glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);

		//cellBorder
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindVertexArray(_vao[CELL_BORDER]);
			glActiveTexture(GL_TEXTURE0);	
			glBindTexture(GL_TEXTURE_2D, context::_options._labelScheme);
				glActiveTexture(GL_TEXTURE1);	
				glBindTexture(GL_TEXTURE_2D, evalTex);
					_c_shader_ptr->use();		
						glUniform1i(_c_shader_ptr->getUniformLocation("width"), w);
						glUniform1i(_c_shader_ptr->getUniformLocation("height"), h);
						glUniform1i(_c_shader_ptr->getUniformLocation("colorScheme"), 0);
						glUniform1i(_c_shader_ptr->getUniformLocation("evalField"), 1);
						glUniform2f(_c_shader_ptr->getUniformLocation("move"), (float)(xShift+xMove)/(float)w*2.0f, -(float)(yShift+yMove)/(float)h*2.0f);
						glUniform2f(_c_shader_ptr->getUniformLocation("mouseCoords"), _clickX, _clickY);
						glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); 
					_c_shader_ptr->unUse();
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);

		glm::mat4 MVP_1(1.0f);

		//box
		glBlendFunc(GL_ONE, GL_ZERO);
		glBindVertexArray(_vao[BOX]);
			_b_shader_ptr->use();			
				glUniformMatrix4fv(_l_shader_ptr->getUniformLocation("MVP"), 1, GL_FALSE, glm::value_ptr(MVP_1));
				glUniform4f(_b_shader_ptr->getUniformLocation("color"), 0.9f, 0.9f, 0.9f, 1.0f);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); 
			_b_shader_ptr->unUse();
		glBindVertexArray(0);

		//text
		for (int i = 0; i < 5; i++) {
			_renderer[i]->renderText();
		}
	}
}


//private stuff

void LabelSelectionPainter::createShader( void ) {
	if (_l_shader_ptr == NULL) {
		vector<string> unis;
		vector<string> attribs;

		attribs.push_back("vVertex");
		unis.push_back("color");
		unis.push_back("MVP");
		_b_shader_ptr = new GLSLShader(attribs, unis, "shaders/standard/shader.vert", "shaders/standard/shader.frag");

		unis.clear();
		unis.push_back("MVP");
		unis.push_back("colorScheme");
		_l_shader_ptr = new GLSLShader(attribs, unis, "shaders/labelLines/shader.vert", "shaders/labelLines/shader.frag", "shaders/labelLines/shader.gem");

		unis.clear();
		attribs.push_back("vTex");
		unis.push_back("mouseCoords");
		unis.push_back("move");
		unis.push_back("colorScheme");
		unis.push_back("evalField");
		unis.push_back("width");
		unis.push_back("height");
		_c_shader_ptr = new GLSLShader(attribs, unis, "shaders/cellBorder/shader.vert", "shaders/cellBorder/shader.frag");
	}
}

void LabelSelectionPainter::sortLabels(vector<Label>* unsorted) {
	struct labelCompare {
		bool operator() (Label l1, Label l2) {
			return (l1.weight > l2.weight);
		}
	} comparator;

	sort(unsorted->begin(), unsorted->end(), comparator);
}

void LabelSelectionPainter::prepareTextRenderer(int* retXSize, int* retYSize, vector<float>* retYAnchor, vector<Label>* sortedLabels) {
	vector<Label> topX;

	mouseHandler::clearLB();
	int windowW,windowH;
	context::getWindowSize(&windowW, &windowH);
	int i = 0;

	//select top x labels
	while (topX.size() < context::_options._labelCount && i < sortedLabels->size()) {
		topX.push_back(sortedLabels->at(i));
		i++;
	}

	float maxW = topX.front().weight;
	float minW = topX.back().weight;
	float div = maxW - minW;

	vector<int> topX2RenderIndex;

	//add them to the correct renderer
	for (int i = 0; i < topX.size(); i++) {
		float normedVal;

		if (div != 0) {
			normedVal = (topX[i].weight - minW)  / div;
		} else {
			normedVal = 0.5f;
		}
		
		ScaleOptions* so = &context::_scaleOptions[2];
		float pointsX[4] = {so->_controlPoints[0][0], so->_controlPoints[1][0], so->_controlPoints[2][0], so->_controlPoints[3][0]};
		float pointsY[4] = {so->_controlPoints[0][1], so->_controlPoints[1][1], so->_controlPoints[2][1], so->_controlPoints[3][1]};

		float scaled = scale(normedVal, so->_linearMode, so->_exponent, pointsX, pointsY);

		int index = min(4, (int)floor(scaled * 5.0f));
		stringstream text;
		text << topX[i].text;
		text << " (";
		text << topX[i].weight;
		text << ")";
		_renderer[index]->addText(text.str(), normedVal);
		topX2RenderIndex.push_back(index);
	}

	//calculate positions
	int maxPixWidth = -1;
	int yOff = Y_MARGIN;

	int off[5] = {0,0,0,0,0};
	for (int i = 0; i < topX.size(); i++) {
		int rI = topX2RenderIndex[i];
		
		const vector<PreparedText* >* texts = _renderer[rI]->getTexts();
		int w = texts->at(off[rI])->_textPixWidth;
		int h = texts->at(off[rI])->_textPixHeight;

		float y = 1.0f - ((float)yOff + (float)h / 2.0f) / (float) windowH;
		float x = ((float)w / 2.0f + (float)X_MARGIN) / (float) windowW;
	
		if (w > maxPixWidth) {
			maxPixWidth = w;
		}
		yOff += (h + Y_MARGIN);
		retYAnchor->push_back(y * 2.0f - 1.0f);

		_renderer[rI]->setCenter(off[rI], x, y);
		off[rI]++;

		//update click listener
		float left = x - ((float) w / (float) windowW) / 2.0f;
		float right = x + ((float) w / (float) windowW) / 2.0f;
		float bottom = y - ((float) h / (float) windowH) / 2.0f;
		float up = y + ((float) h / (float) windowH) / 2.0f;

		mouseHandler::registerLB(bottom, left, up, right, topX[i].id);

	
	}

	*retYSize = yOff;
	*retXSize = (2 * X_MARGIN + maxPixWidth);
}

float LabelSelectionPainter::scale(float normedVal, bool linearMode, float exponent, float pointsX[], float pointsY[]) {
	if (linearMode) {
		if (normedVal == 1.0f) {
			//avoid 0 division
			return 1.0f;
		} else {
			if (normedVal >= pointsX[3]) {
				return mix(pointsY[3], 1.0, ((normedVal - pointsX[3]) / (1.0f - pointsX[3])));
			} else 
			if (normedVal >= pointsX[2]) {
				return mix(pointsY[2], pointsY[3], ((normedVal - pointsX[2]) / (pointsX[3] - pointsX[2])));
			} else 
			if (normedVal >= pointsX[1]) {
				return mix(pointsY[1], pointsY[2], ((normedVal - pointsX[1]) / (pointsX[2] - pointsX[1])));
			} else 
			if (normedVal >= pointsX[0]) {
				return mix(pointsY[0], pointsY[1], ((normedVal - pointsX[0]) / (pointsX[1] - pointsX[0])));
			} else {
				return mix(0.0f, pointsY[0], (normedVal / pointsX[0]));
			}
		}
	} else {
		//exponential mode
		return pow(normedVal, exponent); 	
	}
}

float LabelSelectionPainter::mix(float x, float y, float a) {
	return (x * (1.0f-a) + y * a);
}


void LabelSelectionPainter::initVao( void ) {
	
	glGenVertexArrays(3, &_vao[0]);
	glGenBuffers(4, &_vbo[0]);

	glBindVertexArray(_vao[BOX]);	 
		glBindBuffer(GL_ARRAY_BUFFER, _vbo[BOX]);
			glEnableVertexAttribArray(_b_shader_ptr->getAttributeLocation("vVertex"));
			glVertexAttribPointer (_b_shader_ptr->getAttributeLocation("vVertex"), 2, GL_FLOAT, GL_FALSE, 0, 0);	
		glBindBuffer (GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glBindVertexArray(_vao[LINES]);	 
		glBindBuffer(GL_ARRAY_BUFFER, _vbo[LINES]);
			glEnableVertexAttribArray(_l_shader_ptr->getAttributeLocation("vVertex"));
			glVertexAttribPointer (_l_shader_ptr->getAttributeLocation("vVertex"), 4, GL_FLOAT, GL_FALSE, 0, 0);	
		glBindBuffer (GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//cell shader

	float quad[8] = { -1.0f, 1.0f,    -1.0f, -1.0f,		 1.0f, 1.0f, 	 1.0f, -1.0f};
	float texture[8] = { 0.0f, 1.0f,     0.0f,  0.0f,      1.0f, 1.0f,     1.0f,  0.0f};

	glBindBuffer(GL_ARRAY_BUFFER, _vbo[CELL_BORDER + VERTEX]);
		glBufferData (GL_ARRAY_BUFFER, 8 * sizeof(float), &quad[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[CELL_BORDER + TEX]);
		glBufferData (GL_ARRAY_BUFFER, 8 * sizeof(float), &texture[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	

	glBindVertexArray(_vao[CELL_BORDER]);	 
		glBindBuffer(GL_ARRAY_BUFFER, _vbo[CELL_BORDER + VERTEX]);
			glEnableVertexAttribArray(_c_shader_ptr->getAttributeLocation("vVertex"));
			glVertexAttribPointer (_c_shader_ptr->getAttributeLocation("vVertex"), 2, GL_FLOAT, GL_FALSE, 0, 0);	
		glBindBuffer(GL_ARRAY_BUFFER, _vbo[CELL_BORDER + TEX]);			
			glEnableVertexAttribArray(_c_shader_ptr->getAttributeLocation("vTex"));
			glVertexAttribPointer (_c_shader_ptr->getAttributeLocation("vTex"), 2, GL_FLOAT, GL_FALSE, 0, 0);	
		glBindBuffer (GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}