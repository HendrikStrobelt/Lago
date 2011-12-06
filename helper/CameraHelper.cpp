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

};