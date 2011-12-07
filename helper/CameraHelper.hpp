#ifndef CAMERA_HELPER_H
#define CAMERA_HELPER_H

#include "../dataAccess/NodeStructureInfoContainer.hpp"
#include <glm\glm.hpp>

namespace cameraHelper {

	#define BORDER_FACTOR 1.4f

	glm::mat4 calculateProjection(const NodeStructureInfoContainer* nodeInfo, float zoomFactor);
	
	float getPixelSize(const NodeStructureInfoContainer* nodeInfo, float zoomFactor);

	void mouseDist2StandardVolDist(float* distX, float* distY, int mouseDistX, int mouseDistY);
	void mouseDist2WorldDist(float* distX, float* distY, int mouseDistX, int mouseDistY);
};


#endif
