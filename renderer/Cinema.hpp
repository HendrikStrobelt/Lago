#ifndef CINEMA_H
#define CINEMA_H

#include "../context/IContextListener.hpp"
#include <vector>
using namespace std;



enum TYPE {MOVE_PRESSED, PAN, SIDE_EXP, ZOOM_EXP};

struct RenderCommand {
	TYPE type;
	bool valid;
	float startTime;
	float endTime; //optional except for move pressed
	vector<int> intParas;
	vector<int> floatParas;
	RenderCommand(TYPE ptype, float pStartTime) : type(ptype), valid(true), startTime(pStartTime) {};
};

class Cinema : public IContextListener {

	public:
		Cinema( void );
		~Cinema( void );
		
		void run( void );

		//handler
		void keyEvent(int key, int action);

		//cin cmds
		void addPressedMovement(float startTime, float endTime, int targetMouseX, int targetMouseY);
		void addPanning(float time, float worldDistX, float worldDistY);
		void addSideChange(float time, int newSideExp);
		void addZoomChange(float time, int newZoomExp);

	private:

		void setPressedMovement(int mouseX, int mouseY);
		void setPanning(float worldDistX, float worldDistY);
		void setSideExponent(int exp);
		void setZoomExponent(int exp);

		bool _runCinema;
		double _runStart;
		vector<RenderCommand> _cmds;

};

#endif