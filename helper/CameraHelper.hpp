#ifndef CAMERA_HELPER_H
#define CAMERA_HELPER_H

#include "../dataAccess/NodeStructureInfoContainer.hpp"
#include <glm\glm.hpp>

namespace cameraHelper {

	#define BORDER_FACTOR 1.4f

	glm::mat4 calculateProjection(const NodeStructureInfoContainer* nodeInfo, float zoomFactor);
	
	float getPixelSize(const NodeStructureInfoContainer* nodeInfo, float zoomFactor);

	void mouse2World(float* worldX, float* worldY, int mouseX, int mouseY);
};


#endif
