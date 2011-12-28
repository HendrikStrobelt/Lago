#ifndef LABEL_SELECTION_PAINTER_H
#define LABEL_SELECTION_PAINTER_H

#include "../Label.hpp"
#include "../helper/GLSLShader.hpp"
#include "../text/TextRenderer.hpp"

class LabelSelectionPainter {

	public:
		LabelSelectionPainter( void );
		~LabelSelectionPainter( void );

		void clear( void );
		void setData(vector<int>* ids, const vector<Label>* indexedLabels);
		void renderSelection( void );

		//static clean up
		static void cleanUp( void );
	private:
		enum PAINTER { BOX, LINES };

		static GLSLShader* _b_shader_ptr;
		static GLSLShader* _l_shader_ptr;
		static void createShader( void );
		
		void sortLabels(vector<Label>* unsorted);
		void initVao( void );
		void prepareTextRenderer(vector<Label>* sortedLabels);
		float scale(float normedVal, bool linearMode, float exponent, float pointsX[], float pointsY[]);
		float mix(float x, float y, float a);
		
		TextRenderer* _renderer[5];
		bool _active;
		int _lines;

		GLuint _vao[2];
		GLuint _vbo[2];
};

#endif