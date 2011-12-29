#include "Renderer.hpp"
#include "../GlobalConstants.hpp"
#include "../context/Context.hpp"
#include "../helper/CameraHelper.hpp"
#include "../helper/MouseHandler.hpp"
#include "../Node.hpp"

#include <glm/gtc/matrix_transform.hpp>

Renderer::Renderer( void ) {
	_currentData = new RenderData;
	_newData = new RenderData;
	_cellLabelGetter = NULL;

	glGenBuffers(1, &_nodeVBO);
	glGenBuffers(1, &_edgeVBO);
	context::getWindowSize(&_windowWidth, &_windowHeight);

	//load initial data
	setNewData("_Data/LineNode.out");
	context::_pixelSize = cameraHelper::getPixelSize(_dCache.getNodeStructureInfo(), context::_zoomFactor);

	_mouseMoveX = 0;
	_mouseMoveY = 0;
	_hasEdges = 0;

	_idle = new Idle(this);
	_initalWork = new InitialWork(this);
	_working = new Working(this);
	_visAdjust = new VisAdjusting(this);

	setState(_initalWork);
}

Renderer::~Renderer( void ) {
	glDeleteBuffers(1, &_nodeVBO);
	glDeleteBuffers(1, &_edgeVBO);

	delete _currentData;
	delete _newData;
	delete _idle;
	delete _initalWork;
	delete _working;
	delete _visAdjust;
	delete _cellLabelGetter;
}


//public

void Renderer::rightClick(int x, int y) {
	cout << "x " << x << "y " << y << "\n";

	glm::mat4 P = cameraHelper::calculateProjection(_dCache.getNodeStructureInfo(), context::_zoomFactor);
	glm::mat4 MVP = glm::translate(P, glm::vec3(context::_worldTransX, context::_worldTransY, 0.0f));

	vector<int>* ids = _cellLabelGetter->getLabelIndices(x, y, _currentData->_evalField, MVP);

	_labelSelectionPainter.setData(ids, _dCache.getIndexedLabels());
}


//private methods that can be used by the states

void Renderer::setState(IRenderState* state) {
	_state = state;
	_state->takeOver();
}

void Renderer::setNewData(string nodeFile, string edgeFile) {
	_dCache.loadDataSet(nodeFile, edgeFile);

	delete _cellLabelGetter;

	int nodeCount;
	const PackedNode* packedNodes = _dCache.getPackedNodes(&nodeCount);
	glBindBuffer(GL_ARRAY_BUFFER, _nodeVBO);
		glBufferData(GL_ARRAY_BUFFER, nodeCount * sizeof(PackedNode), packedNodes, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (!edgeFile.empty()) {
		_hasEdges = true;
		int edgeCount;
		const PackedEdge* packedEdges = _dCache.getPackedEdges(&edgeCount);
		glBindBuffer(GL_ARRAY_BUFFER, _edgeVBO);
			glBufferData(GL_ARRAY_BUFFER, edgeCount * sizeof(PackedEdge), packedEdges, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	} else {
		_hasEdges = false;
	}

	_cellLabelGetter = new CellLabelGetter(_nodeVBO, _dCache.getNodeStructureInfo()->getAllNodes(_dCache.getNodeStructureInfo()->getMaxDepth()));
}	

void Renderer::updateLabels(RenderData* rData) {
	if (_dCache.hasLabels() && context::_options._showLabels) {
		glm::mat4 P = cameraHelper::calculateProjection(_dCache.getNodeStructureInfo(), context::_zoomFactor);
		glm::mat4 MVP = glm::translate(P, glm::vec3(context::_worldTransX, context::_worldTransY, 0.0f));
		rData->_labelPainter.changeLabels(MVP, _dCache.getIndexedLabels());
	}
}

void Renderer::renderGraph(RenderData* rData, int xMove, int yMove) {
	int mouseMoveX, mouseMoveY;
	mouseHandler::getPressMovement(&mouseMoveX, &mouseMoveY);

	mouseMoveX = mouseMoveX + xMove;
	mouseMoveY = mouseMoveY + yMove;

	float moveX, moveY;
	cameraHelper::mouseDist2StandardVolDist(&moveX, &moveY, mouseMoveX, mouseMoveY);
	float maxValues[] = {1.0f, 1.0f, 1.0f}; //vis tex is already normalized

	renderTexture(rData->_vis, maxValues, moveX, moveY);
	renderLabels(rData, mouseMoveX, -mouseMoveY);
}

void Renderer::renderHUD(float progress, float maxVals[]) {
	if (progress >= 0.0f) {
		if (_hasEdges) {
			_progressBar.renderBar(progress, 3);
		} else {
			_progressBar.renderBar(progress, 1);
		}
	}

	if (maxVals[0] > 0.0f) {
		_scalingBars.renderScaleBars(maxVals, _hasEdges);
	}

	glm::mat4 P = cameraHelper::calculateProjection(_dCache.getNodeStructureInfo(), context::_zoomFactor);
	glm::mat4 MVP = glm::translate(P, glm::vec3(context::_worldTransX, context::_worldTransY, 0.0f));

	int xShift, yShift;
	mouseHandler::getPressMovement(&xShift, &yShift);
	_labelSelectionPainter.renderSelection(MVP, xShift, yShift);
}

void Renderer::renderLabels(RenderData* rData, int xMove, int yMove) {
	if (_dCache.hasLabels() && 	context::_options._showLabels)  {
		rData->_labelPainter.renderLabels(xMove, yMove);
	}
}

void Renderer::renderTexture(GLuint tex, float max[], float xMove, float yMove) {
	_displayConvert.renderTexture(tex, max, xMove, yMove);
}

void Renderer::calculateMaxValues(float result[], GLuint texture, int textureWidth, int textureHeight) {
	_textureExaminer.getMaxValues(result, texture, textureWidth, textureHeight, TE_KERNEL_SIZE, TE_MAX_ITERATIONS);
}


//state methods
void Renderer::render( void ) {
	_state->render();
}

void Renderer::renderGauss( void ) {
	_state->renderGauss();
}

void Renderer::renderEvalField( void ) {
	_state->renderEvalField();
}

void Renderer::renderLineField( void ) {
	_state->renderLineField();
}

void Renderer::work( void ) {
	_state->work();
}

void Renderer::takeOver( void ) {
	//state only 
}

void Renderer::changePanning(int xMouseMove, int yMouseMove) {
	_state->changePanning(xMouseMove, yMouseMove);
}

void Renderer::changeZoom( void ) {
 	context::_pixelSize = cameraHelper::getPixelSize(_dCache.getNodeStructureInfo(), context::_zoomFactor);
	_state->changeZoom();
}

void Renderer::changeData(string nodeFile, string edgeFile) {
	setNewData(nodeFile, edgeFile);
	context::_pixelSize = cameraHelper::getPixelSize(_dCache.getNodeStructureInfo(), context::_zoomFactor);
	_state->changeData(nodeFile, edgeFile);
}

void Renderer::changeSideLength( void ) {
	_state->changeSideLength();
}

void Renderer::changeWindow( void ) {
	context::getWindowSize(&_windowWidth, &_windowHeight);
	context::_pixelSize = cameraHelper::getPixelSize(_dCache.getNodeStructureInfo(), context::_zoomFactor);
	_state->changeWindow();
}

void Renderer::changeVisParameter( void ) {
	_state->changeVisParameter();
}