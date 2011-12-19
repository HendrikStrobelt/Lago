#ifndef LABEL_PAINTER_H
#define LABEL_PAINTER_H

#include "../context/IContextListener.hpp"
#include "../text/TextRenderer.hpp"
#include "../Label.hpp"

#include <glm\glm.hpp>

class LabelPainter {

	public:
		LabelPainter( void );
		~LabelPainter( void );

		void changeLabels(glm::mat4 MVP, const vector<Label>* sortedLabels); 
		void renderLabels(int xShift=0, int yShift=0);

		void clear( void );

	private:
		TextRenderer* _renderer[5];
};


#endif