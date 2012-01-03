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

		void updateMVP(glm::mat4 MVP);
		void addLabel(glm::mat4 MVP, Label l);
		void removeLabel(glm::mat4 MVP, Label l);
		void renderLabels(int xShift=0, int yShift=0);
		void clear( void );

	private:
		vector<Label> _labels;

		void clearRenderer( void );
		void changeLabels(glm::mat4 MVP); 
		void sortLabels(vector<Label>* unsorted);

		float scale(float normedVal, bool linearMode, float exponent, float pointsX[], float pointsY[]);
		float mix(float x, float y, float a);

		TextRenderer* _renderer[5];
};


#endif