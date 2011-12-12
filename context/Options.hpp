#ifndef OPTIONS_H
#define OPTIONS_H

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

		//implemented IContextListener methods
		void keyEvent(int key, int action);

		//pull
		bool _antiAlias;

		bool _changedLocal;
	

	private:
		string _nodeFile;
		string _edgeFile;

		void dataChanged(string nodeFile, string edgeFile="");
		

};

#endif