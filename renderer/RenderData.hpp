#ifndef RENDER_DATA_H
#define RENDER_DATA_H

#include "IRenderData.hpp"
#include <GL/glew.h>
#include <GL/glfw.h>

class RenderData : public IRenderData {

	public:
		RenderData( void );
		~RenderData( void );

		//implement interface
		void clear( void );
		GLuint getGaussTex( void );
		GLuint getEvalField( void );
		GLuint getLineField( void );
		GLuint getVis( void );
		float getNodeMax( void );
		float getEdgeMax( void );

		float* getNodeMaxAll( void );
		float* getEdgeMaxAll( void );

		float* getBox( void );
		float getSideLength( void );

		//add setters and members
		void setGaussTex(GLuint tex);
		void setEvalField(GLuint tex);
		void setLineField(GLuint tex);
		void setVis(GLuint tex);
		void setNodeMax(float max[]);
		void setEdgeMax(float max[]);
		void setBox(float box[]);
		void setSideLength(float length);

	private:
		GLuint _gaussTex;
		GLuint _evalField;
		GLuint _lineField;
		GLuint _vis;

		float _maxValuesN[3];
		float _maxValuesE[3];
		float _box[4];
		float _sideLength;
};


#endif