#ifndef RENDER_BLEND_DATA_H
#define RENDER_BLEND_DATA_H

#include "IRenderData.hpp"
#include "RenderData.hpp"
#include <GL/glew.h>
#include <GL/glfw.h>

class RenderBlendData : public IRenderData {
		
	public:
		RenderBlendData(RenderData* rData);
		~RenderBlendData( void );

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

		//add setters and members
		void setNodeMax(float max[]);
		void setEdgeMax(float max[]);
		void setVis(GLuint tex);


	private:
		RenderData* _rData;
		GLuint _vis;

		float _maxValuesN[3];
		float _maxValuesE[3];
};


#endif