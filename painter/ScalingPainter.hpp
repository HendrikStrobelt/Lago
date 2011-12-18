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
		enum DATA {VERTEX, TEX_COORDS};
		
		static GLSLShader* _shader_ptr;
		static void createShader( void );
		
		TextRenderer* test;

		void updateBarPositions(int w, int h);
		string getFloatString(float value);
		float reverseScale(float normedVal, bool linearMode, float exponent, float pointsX[], float pointsY[]);
		float mix(float x, float y, float a);

		void getShader( void );
		void initVao( void );

		float _textAnchors[6][2];
		float _maxE;
		float _maxN;
		float _n05;
		float _e05;

		GLuint _vao;
		GLuint _vbo[2];
};

#endif