#ifndef OPTIONS_H
#define OPTIONS_H

#include <GL/glew.h>
#include <GL/glfw.h>
#include "IContextListener.hpp"
#include <map>
#include <string>

using namespace std;

class Options : public IContextListener {

	public:
		Options( void );
		~Options( void );

		void update(map<string, string> dataMap);
		string toCommandString( void );

		void init( void );

		//implemented IContextListener methods
		void keyEvent(int key, int action);

		//pull
		bool _antiAlias;
		bool _adaptiveScaleBars;
		GLuint _nodeScheme;
		GLuint _edgeScheme;

		bool _changedLocal;
	

	private:
		string _nodeFile;
		string _edgeFile;
		string _colorSchemeEdge;
		string _colorSchemeNode;


		void dataChanged(string nodeFile, string edgeFile="");
		void loadTextures( void );


};

#endif