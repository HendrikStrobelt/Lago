#include "LabelPainter.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "../context/Context.hpp"

LabelPainter::LabelPainter( void ) {
	_renderer[5] = new TextRenderer("C://Windows//fonts//timesbd.ttf", 24);
	_renderer[4] = new TextRenderer("C://Windows//fonts//timesbd.ttf", 16);
	_renderer[3] = new TextRenderer("C://Windows//fonts//timesbd.ttf", 12);
	_renderer[2] = new TextRenderer("C://Windows//fonts//timesbd.ttf", 8);
	_renderer[1] = new TextRenderer("C://Windows//fonts//timesbd.ttf", 6);
	_renderer[0] = new TextRenderer("C://Windows//fonts//timesbd.ttf", 4);
}


LabelPainter::~LabelPainter( void ) {
	for (int i = 0; i < 6; i++) {
		delete _renderer[i];
	}
}

//public stuff

void LabelPainter::clear( void ) {
	for (int i = 0; i < 6; i++) {
		_renderer[i]->clearTextStorage();
	}
}

void LabelPainter::changeLabels(glm::mat4 MVP, const vector<Label>* sortedLabels) {
	vector<Label> topX;

	int i = 0;

	glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 0.0f));
	glm::mat4 S = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 1.0f));
	glm::mat4 MVP2 = S * T * MVP; // transforms world to 0..1-0..1 coordinates

	while (topX.size() < MAX_LABELS && i < sortedLabels->size()) {
		glm::vec4 labelPos(sortedLabels->at(i).x, sortedLabels->at(i).y, 0.0f, 1.0f);
		labelPos = MVP2 * labelPos;

		if (labelPos.x >= 0.0f && labelPos.x <= 1.0f && labelPos.y >= 0.0f && labelPos.y <= 1.0f) {
			//inside
			topX.push_back(sortedLabels->at(i));
			topX.back().x = labelPos.x;
			topX.back().y = labelPos.y;
			
			int a = strlen(topX.back().text);

			float normedVal = topX.back().weight / topX.front().weight;
			ScaleOptions* so = &context::_scaleOptions[2];
			float pointsX[4] = {so->_controlPoints[0][0], so->_controlPoints[1][0], so->_controlPoints[2][0], so->_controlPoints[3][0]};
			float pointsY[4] = {so->_controlPoints[0][1], so->_controlPoints[1][1], so->_controlPoints[2][1], so->_controlPoints[3][1]};

			float scaled = scale(normedVal, so->_linearMode, so->_exponent, pointsX, pointsY);
			int index = min(5, (int)floor(scaled * 6.0f));

			_renderer[index]->addText(topX.back().text, labelPos.x, labelPos.y, normedVal);
		}

		i++;
	}

	//add them to the correct renderer
	
}


void LabelPainter::renderLabels(int xShift, int yShift) {
	for (int i = 0; i < 6; i++) {
		_renderer[i]->renderText(xShift, yShift);
	}
}


//private

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
