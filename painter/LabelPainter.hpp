#ifndef LABEL_PAINTER_H
#define LABEL_PAINTER_H

#include "../context/IContextListener.hpp"
#include "../text/TextRenderer.hpp"
#include "../Label.hpp"

#include <glm\glm.hpp>

class LabelPainter : IContextListener {

	public:
		LabelPainter( void );
		~LabelPainter( void );

		void changeLabels(glm::mat4 MVP, const vector<Label>* sortedLabels); 

		void renderLabels(float xShift=0.0f, float yShift=0.0f);

		//interface method
		void resizeEvent(int width, int height);

	private:
		TextRenderer* _renderer[5];
};


#endif