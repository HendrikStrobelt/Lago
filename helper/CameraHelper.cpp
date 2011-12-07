#include "CameraHelper.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "../context/Context.hpp"

namespace cameraHelper {

glm::mat4 calculateProjection(const NodeStructureInfoContainer* nodeInfo, float zoomFactor) {

	float minX = nodeInfo->getLeftBorder();
	float minY = nodeInfo->getBottomBorder();
	float width = nodeInfo->getWidth();
	float height = nodeInfo->getHeight();
	int w,h;
	context::getWindowSize(&w, &h);
	float ratio = (float)w / (float)h;
	
	float stdWidth, stdHeight, viewWidth, viewHeight, viewLeft, viewRight, viewBottom, viewTop;

	if (height * ratio > width) {
		//y dominates
		stdHeight = height * BORDER_FACTOR;
		stdWidth = stdHeight * ratio;
	} else {
		//x dominates
		stdWidth = width * BORDER_FACTOR;
		stdHeight = stdWidth / ratio;
	}

	viewHeight = stdHeight / zoomFactor;
	viewWidth = stdWidth / zoomFactor;

	viewLeft = (minX + (width / 2.0f)) - (viewWidth/2.0f);
	viewRight = viewLeft + viewWidth;
	viewBottom = (minY + (height / 2.0f)) - (viewHeight/2.0f);
	viewTop = viewBottom + viewHeight;

	return glm::ortho<float>(viewLeft, viewRight, viewBottom, viewTop, -1, 1);
}


float getPixelSize(const NodeStructureInfoContainer* nodeInfo, float zoomFactor) {
	float minX = nodeInfo->getLeftBorder();
	float minY = nodeInfo->getBottomBorder();
	float width = nodeInfo->getWidth();
	float height = nodeInfo->getHeight();
	int w,h;
	context::getWindowSize(&w, &h);
	float ratio = (float)w / (float)h;
	
	float stdWidth, stdHeight, viewWidth, viewHeight, viewLeft, viewRight, viewBottom, viewTop;
	float result;

	if (height * ratio > width) {
		//y dominates
		stdHeight = height * BORDER_FACTOR ;
		viewHeight = stdHeight / zoomFactor;

		result = viewHeight / (float)h;
	} else {
		//x dominates
		stdWidth = width * BORDER_FACTOR;
		viewWidth = stdWidth / zoomFactor;

		result = viewWidth / (float)w;
	}
	return result;
}


void mouse2World(float* worldX, float* worldY, int mouseX, int mouseY) {
	int w,h;
	context::getWindowSize(&w, &h);

	float retX,retY;
	retX = (float)mouseX / (float)w;
	retY = (float)mouseY / (float)h;
	
	retX *=  2.0f;
	retY *= -2.0f;

	retX -= 1.0f;
	retY += 1.0f;

	*worldX = retX;
	*worldY = retY;
}

};