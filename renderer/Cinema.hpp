#ifndef CINEMA_H
#define CINEMA_H

#include "../context/IContextListener.hpp"
#include <vector>
using namespace std;


#define CAPTURE_MODE true
#define FRAME_TIME (1.0/30.0)

enum TYPE {MOVE_PRESSED_M, MOVE_PRESSED, PAN, PAN_M, SIDE_EXP, ZOOM_EXP, EMPTY};

struct RenderCommand {
	TYPE type;
	bool valid;
	float startTime;
	float endTime; //optional except for move pressed
	vector<int> intParas;
	vector<float> floatParas;
	RenderCommand(TYPE ptype, float pStartTime) : type(ptype), valid(true), startTime(pStartTime) {};
};

class Cinema : public IContextListener {

	public:
		Cinema( void );
		~Cinema( void );
		
		void run(double workTime);
		void rendered( void );

		//handler
		void keyEvent(int key, int action);

		//cin cmds
		void addPressedMovement(float startTime, float endTime, float targetX, float targetY);
		void addPressedMovementM(float startTime, float endTime, int targetMouseX, int targetMouseY);
		void addPanning(float time, float worldDistX, float worldDistY);
		void addPanningM(float time, int mouseX, int mouseY);
		void addSideChange(float time, int newSideExp);
		void addZoomChange(float time, int newZoomExp);
		void addEmptyEvent(float time);
		static double _captureTime;

	private:

		void setPressedMovement(int mouseX, int mouseY);
		void setPanning(int mouseX, int mouseY);
		void setSideExponent(int exp);
		void setZoomExponent(int exp);

		bool _runCinema;
		double _runStart;
		double _lastFrameTime;
		int _imageID;
		vector<RenderCommand> _cmds;

};

#endif