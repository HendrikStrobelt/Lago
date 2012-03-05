#include "Cinema.hpp"
#include "../context/Context.hpp"
#include "../helper/MouseHandler.hpp"
#include "../helper/CameraHelper.hpp"
#include "../helper/EnvironmentHelper.hpp"
#include <iostream>
#include <sstream>

double Cinema::_captureTime = -1.0;

Cinema::Cinema( void ) {
	context::addKeyEventListener(this);
	_runCinema = false;
}

Cinema::~Cinema( void ) {

}


void Cinema::run(double workTime) {
	if (_runCinema) {

		double runTime;

		if (CAPTURE_MODE) { 
			double currentTime = _lastFrameTime;	

			do {
				stringstream ss;
				ss << "D://film//";
				ss << _imageID;
				ss << ".tga";
				_imageID++;

				envHelper::writeTexture2TGA(Renderer::_tex, ss.str());
				currentTime += FRAME_TIME;
				cout << currentTime << "\n";
			} while ((_lastFrameTime + workTime) > currentTime);

			_lastFrameTime = currentTime;
			runTime = currentTime;
			_captureTime = runTime;
		} else {
			runTime = glfwGetTime() - _runStart;
		}
		
		bool foundOne = false;
		for (int i = 0; i < _cmds.size(); i++) {

			if (_cmds[i].valid) {
				foundOne = true;
			}


			if (_cmds[i].startTime <= runTime && _cmds[i].valid) {
				if (_cmds[i].type == MOVE_PRESSED) {
					if (runTime > _cmds[i].endTime) {
						_cmds[i].valid = false;
					} else {
						float percent = (runTime - _cmds[i].startTime) / (_cmds[i].endTime - _cmds[i].startTime);
						setPressedMovement(_cmds[i].intParas[0] * percent, _cmds[i].intParas[1] * percent);
					}
				} else 
				if (_cmds[i].type == PAN) {
					_cmds[i].valid = false;
					setPanning(_cmds[i].floatParas[0] - context::_worldTransX, _cmds[i].floatParas[1] - context::_worldTransY);
				} else 
				if (_cmds[i].type == SIDE_EXP) {
					_cmds[i].valid = false;
					setSideExponent(_cmds[i].intParas[0]);
				} else 
				if (_cmds[i].type == ZOOM_EXP) {
					_cmds[i].valid = false;
					setZoomExponent(_cmds[i].intParas[0]);
				} else
				if (_cmds[i].type == EMPTY) {
					_cmds[i].valid = false;
				}
			}
		}

		if (!foundOne) {
			//all invalid
			_runCinema = false;
			_captureTime = -1.0;
		}
	}
}


void Cinema::rendered( void ) {
	setPressedMovement(0,0);
}

void Cinema::keyEvent(int key, int action) {

	if (action == GLFW_RELEASE) {
		if (key == 'R') {
			int x,y;
			mouseHandler::getPressMovement(&x,&y);
			cout << "pressed movement x: " << x << " y: " << y << "\n";
			cout << "world translate x: " << context::_worldTransX << " y: " << context::_worldTransY << "\n";
			cout << "side exponent: " << context::_sideExponent << "\n";
			cout << "zoom exponent: " << context::_zoomExponent << "\n";
		} else if (key == 'P') {
			for (int i = 0; i < _cmds.size(); i++) {
				_cmds[i].valid = true;
			}
			_imageID = 1;
			_runCinema = true;
			_runStart = glfwGetTime();
			_lastFrameTime = 0;

			if (CAPTURE_MODE) {
				_captureTime = 0.0;
			}
		}
	}

}


void Cinema::addPressedMovement(float startTime, float endTime, int targetMouseX, int targetMouseY) {
	RenderCommand rc(MOVE_PRESSED, startTime);
	rc.endTime = endTime;
	rc.intParas.push_back(targetMouseX);
	rc.intParas.push_back(targetMouseY);

	_cmds.push_back(rc);
}

void Cinema::addPanning(float time, float worldX, float worldY) {
	RenderCommand rc(PAN, time);
	rc.floatParas.push_back(worldX);
	rc.floatParas.push_back(worldY);

	_cmds.push_back(rc);
}


void Cinema::addSideChange(float time, int newSideExp) {
	RenderCommand rc(SIDE_EXP, time);
	rc.intParas.push_back(newSideExp);

	_cmds.push_back(rc);
}

void Cinema::addEmptyEvent(float time) {
	RenderCommand rc(EMPTY, time);

	_cmds.push_back(rc);
}

void Cinema::addZoomChange(float time, int newZoomExp) {
	RenderCommand rc(ZOOM_EXP, time);
	rc.intParas.push_back(newZoomExp);

	_cmds.push_back(rc);
}


void Cinema::setPressedMovement(int mouseX, int mouseY) {
	mouseHandler::setMove(mouseX, mouseY);
}

void Cinema::setPanning(float worldDistX, float worldDistY) {
	int x,y;
	cameraHelper::worldDist2MouseDist(&x, &y, worldDistX, worldDistY);
	context::updateWorldTranslate(x,y);
}

void Cinema::setSideExponent(int exp) {
	context::setSideExponent(exp);
}

void Cinema::setZoomExponent(int exp) {
	context::setZoomExponent(exp);
}