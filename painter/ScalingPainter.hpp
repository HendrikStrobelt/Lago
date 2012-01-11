#ifndef SCALING_PAINTER_H
#define SCALING_PAINTER_H

#include "../helper/GLSLShader.hpp"
#include "../renderer/RenderData.hpp"
#include "../context/IContextListener.hpp"
#include "../text/TextRenderer.hpp"
#include <string>

using namespace std;

class ScalingPainter : public IContextListener {

	public:
		ScalingPainter( void );
		~ScalingPainter( void );
		
		void renderScaleBars(float maxVals[], bool hasEdges);

		//interface method
		void resizeEvent(int width, int height);

		//static clean up
		static void cleanUp( void );
	private:
		enum VAO {BAR, LOCK};
		enum DATA {B_VERTEX, B_TEX_COORDS, L_VERTEX, L_TEX_COORDS};
		
		static GLuint _lockTexture;
		static GLSLShader* _b_shader_ptr;
		static GLSLShader* _l_shader_ptr;
		static void createShader( void );
		
		TextRenderer* text;

		void updatePositions(int w, int h);
		string getFloatString(float value);
		float reverseScale(float normedVal, bool linearMode, float exponent, float pointsX[], float pointsY[]);

		void getShader( void );
		void initVao( void );

		float _textAnchors[6][2];
		float _lockXShift;
		float _maxE;
		float _maxN;
		float _n05;
		float _e05;

		GLuint _vao[2];
		GLuint _vbo[4];
};

#endif