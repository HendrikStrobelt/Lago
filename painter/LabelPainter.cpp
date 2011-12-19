#include "LabelPainter.hpp"
#include <glm/gtc/matrix_transform.hpp>

LabelPainter::LabelPainter( void ) {
	_renderer[4] = new TextRenderer("C://Windows//fonts//times.ttf", 36);
	_renderer[3] = new TextRenderer("C://Windows//fonts//times.ttf", 26);
	_renderer[2] = new TextRenderer("C://Windows//fonts//times.ttf", 22);
	_renderer[1] = new TextRenderer("C://Windows//fonts//times.ttf", 18);
	_renderer[0] = new TextRenderer("C://Windows//fonts//times.ttf", 14);
}


LabelPainter::~LabelPainter( void ) {
	for (int i = 0; i < 5; i++) {
		delete _renderer[i];
	}
}

//public stuff

void LabelPainter::clear( void ) {
	for (int i = 0; i < 5; i++) {
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
			_renderer[3]->addText(topX.back().text, labelPos.x, labelPos.y, Color(1.0f, 0.0f, 0.0f));
		}

		i++;
	}

	//add them to the correct renderer
	
}


void LabelPainter::renderLabels(int xShift, int yShift) {
	for (int i = 0; i < 5; i++) {
		_renderer[i]->renderText(xShift, yShift);
	}
}
