#include "LabelSelectionPainter.hpp"

#include <algorithm>
#include <sstream>
#include <glm\gtc\matrix_transform.hpp>
#include "../context/Context.hpp"
#include "../text/PreparedText.hpp"


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
	if (ids->size() > 0) {
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

		int w,h;
		context::getWindowSize(&w, &h);

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
			lines[i * 4 + 2] = 0.0f;
			lines[i * 4 + 3] = 0.0f;
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

void LabelSelectionPainter::renderSelection( void ) {
	if (_active) {	
		glBindVertexArray(_vao[BOX]);
			_b_shader_ptr->use();			
				glUniform4f(_b_shader_ptr->getUniformLocation("color"), 0.9f, 0.9f, 0.9f, 1.0f);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); 
			_b_shader_ptr->unUse();
		glBindVertexArray(0);

		glBindVertexArray(_vao[LINES]);
			_l_shader_ptr->use();			
				glUniform4f(_l_shader_ptr->getUniformLocation("color"), 0.9f, 0.9f, 0.9f, 1.0f);
				glDrawArrays(GL_LINES, 0, 2 * _lines); 
			_l_shader_ptr->unUse();
		glBindVertexArray(0);

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