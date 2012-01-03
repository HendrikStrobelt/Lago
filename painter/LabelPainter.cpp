#include "LabelPainter.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "../context/Context.hpp"
#include "../helper/GraphVizCom.hpp"
#include "../GlobalConstants.hpp"
#include "../helper/MouseHandler.hpp"
#include <algorithm>

LabelPainter::LabelPainter( void ) {
	_renderer[4] = new TextRenderer("C://Windows//fonts//times.ttf", 28);
	_renderer[3] = new TextRenderer("C://Windows//fonts//times.ttf", 20);
	_renderer[2] = new TextRenderer("C://Windows//fonts//times.ttf", 16);
	_renderer[1] = new TextRenderer("C://Windows//fonts//times.ttf", 12);
	_renderer[0] = new TextRenderer("C://Windows//fonts//times.ttf", 10);
}


LabelPainter::~LabelPainter( void ) {
	for (int i = 0; i < 5; i++) {
		delete _renderer[i];
	}
}

//public stuff

void LabelPainter::clear( void ) {
	_labels.clear();
	clearRenderer();
}

void LabelPainter::updateMVP(glm::mat4 MVP) {
	changeLabels(MVP);
}

void LabelPainter::addLabel(glm::mat4 MVP, Label l) {
	bool notIn = true;
	
	for (int i = 0; i < _labels.size(); i++) {
		if (_labels[i].id == l.id) {
			notIn = false;
		}
	}	
	if (notIn) {
		_labels.push_back(l);
		changeLabels(MVP);
	}
}

void LabelPainter::removeLabel(glm::mat4 MVP, Label l) {
	for (int i = 0; i < _labels.size(); i++) {
		if (_labels[i].id == l.id) {
			_labels.erase(_labels.begin() + i);
			break;
		}
	}
	changeLabels(MVP);
}


void LabelPainter::renderLabels(int xShift, int yShift) {
	for (int i = 0; i < 5; i++) {
		_renderer[i]->renderText(xShift, yShift);
	}
}


//private

void LabelPainter::clearRenderer( void ) {
	int w,h;
	context::getWindowSize(&w, &h);

	for (int i = 0; i < 5; i++) {
		_renderer[i]->clearTextStorage();
		_renderer[i]->resize(w, h);
	}
}

void LabelPainter::changeLabels(glm::mat4 MVP) {
	clearRenderer();
	mouseHandler::clearLabel();

	if (_labels.size() > 0) {
		sortLabels(&_labels);
		vector<Label> topX;

		int windowW,windowH;
		context::getWindowSize(&windowW, &windowH);
		int i = 0;

		glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 0.0f));
		glm::mat4 S = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 1.0f));

		glm::mat4 MVP2 = S * T * MVP; // transforms world to 0..1-0..1 coordinates

		vector<int> x_topX;
		vector<int> y_topX;

		//select top x labels
		while (topX.size() < context::_options._labelCount && i < _labels.size()) {
			glm::vec4 labelPos(_labels[i].x, _labels[i].y, 0.0f, 1.0f);
			labelPos = MVP2 * labelPos;

			if (labelPos.x >= 0.0f && labelPos.x <= 1.0f && labelPos.y >= 0.0f && labelPos.y <= 1.0f) {
				//inside
				topX.push_back(_labels[i]);
				x_topX.push_back(labelPos.x * (float)windowW);
				y_topX.push_back(labelPos.y * (float)windowH);
			}

			i++;
		}

		if (topX.size() > 0) {
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
				_renderer[index]->addText(topX[i].text, normedVal);
				topX2RenderIndex.push_back(index);
			}	
				
			//calculate new positions
			vector<graphVizCom::MovedBox>* newPos = NULL;

			if (USE_GRAPHVIZ) {
				graphVizCom::prepare();

				int off[5] = {0,0,0,0,0};
				for (int i = 0; i < topX.size(); i++) {
					int rI = topX2RenderIndex[i];
					PreparedText* label = _renderer[rI]->getTexts()->at(off[rI]);
					graphVizCom::add(x_topX[i], y_topX[i], label->_textPixWidth, label->_textPixHeight);
					off[rI]++;
				}
				newPos = graphVizCom::transmit();

				for (int i = 0; i < newPos->size(); i++) {
					x_topX[newPos->at(i).id] =  newPos->at(i).x;
					y_topX[newPos->at(i).id] =  newPos->at(i).y;
				}
			}
	
			//set new positions
			int off[5] = {0,0,0,0,0};
			for (int i = 0; i < topX.size(); i++) {
				int rI = topX2RenderIndex[i];

				float x = (float)x_topX[i] / (float)windowW;
				float y =  (float)y_topX[i] / (float)windowH;

				_renderer[rI]->setCenter(off[rI], x, y);

				float w = _renderer[rI]->getTexts()->at(off[rI])->_textPixWidth;
				float h = _renderer[rI]->getTexts()->at(off[rI])->_textPixHeight;

				//update click listener
				float left = x - ((float) w / (float) windowW) / 2.0f;
				float right = x + ((float) w / (float) windowW) / 2.0f;
				float bottom = y - ((float) h / (float) windowH) / 2.0f;
				float up = y + ((float) h / (float) windowH) / 2.0f;

				mouseHandler::registerLabel(bottom, left, up, right, topX[i].id);

				off[rI]++;
			}
	
			delete newPos;
		}
	}
}


float LabelPainter::scale(float normedVal, bool linearMode, float exponent, float pointsX[], float pointsY[]) {
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

float LabelPainter::mix(float x, float y, float a) {
	return (x * (1.0f-a) + y * a);
}


void LabelPainter::sortLabels(vector<Label>* unsorted) {
	struct labelCompare {
		bool operator() (Label l1, Label l2) {
			return (l1.weight > l2.weight);
		}
	} comparator;

	sort(unsorted->begin(), unsorted->end(), comparator);
}