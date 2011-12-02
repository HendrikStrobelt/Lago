#ifndef SCALE_OPTIONS_H
#define SCALE_OPTIONS_H

#include <string>
#include <map>

using namespace std;

class ScaleOptions {

	public:
		ScaleOptions( void );
		~ScaleOptions( void );

		void update(map<string, string> dataMap, string prefix);

		bool _linearMode;
		float _controlPoints[4][2];
		float _exponent;

};

#endif