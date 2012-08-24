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
	glGenBuffers(1, &_edgeProcessedVBO);
	context::getWindowSize(&_windowWidth, &_windowHeight);

	//load initial data
	setNewData("initNodeData.out", "", true);
	context::_pixelSize = cameraHelper::getPixelSize(_dStore.getNodeStructureInfo(), context::_zoomFactor);

	_mouseMoveX = 0;
	_mouseMoveY = 0;
	_hasEdges = 0;

	_idle = new Idle(this);
	_initalWork = new InitialWork(this);
	_working = new Working(this);
	_visAdjust = new VisAdjusting(this);

	_meassuring = false;
	_meassureCount = 0;
	_time = 0.0;
	_timeSum = 0.0;

	setState(_initalWork);
}

Renderer::~Renderer( void ) {
	glDeleteBuffers(1, &_nodeVBO);
	glDeleteBuffers(1, &_edgeVBO);
	glDeleteBuffers(1, &_edgeProcessedVBO);
	delete _currentData;
	delete _newData;
	delete _idle;
	delete _initalWork;
	delete _working;
	delete _visAdjust;
	delete _cellLabelGetter;
}


//public

void Renderer::emptyClick( void ) {
	_labelSelectionPainter.clear();
	_visAdjust->cancelAnimation();
}

void Renderer::lockClick( void ) {
	if (context::_options._lock) {
		_visAdjust->cancelAnimation(); //lock setting terminates animation
	}
}

void Renderer::rightClick(int x, int y) {
	vector<int>* ids = _cellLabelGetter->getLabelIndices(x, y, _currentData->getEvalField(), _currentData->getGaussTex(), getStandardMVP());
	_labelSelectionPainter.setData(ids, _dStore.getIndexedLabels(), x, y);
}

void Renderer::labelClick(bool add, int id) {
	Label l = _dStore.getIndexedLabels()->at(id);
	if (add) {
		_labelPainter.addLabel(getStandardMVP(), l);
	} else {
		_labelPainter.removeLabel(getStandardMVP(), l);
	}
}


//private methods that can be used by the states

void Renderer::setState(IRenderState* state) {
	_state = state;
	_state->takeOver();
}

void Renderer::setNewData(string nodeFile, string edgeFile, bool silent) {
	_dStore.setData(nodeFile, edgeFile);

	delete _cellLabelGetter;
	int nodeCount;
	const PackedNode* packedNodes = _dStore.getPackedNodes(&nodeCount);
	glBindBuffer(GL_ARRAY_BUFFER, _nodeVBO);
		glBufferData(GL_ARRAY_BUFFER, nodeCount * sizeof(PackedNode), packedNodes, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (_dStore.hasEdges()) {
		_hasEdges = true;
		int edgeCount;
		const PackedEdge* packedEdges = _dStore.getPackedEdges(&edgeCount);
		glBindBuffer(GL_ARRAY_BUFFER, _edgeVBO);
			glBufferData(GL_ARRAY_BUFFER, edgeCount * sizeof(PackedEdge), packedEdges, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, _edgeProcessedVBO);
			glBufferData(GL_ARRAY_BUFFER, edgeCount * sizeof(ProcessedEdge), NULL, GL_DYNAMIC_COPY);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	} else {
		_hasEdges = false;
	}

	_cellLabelGetter = new CellLabelGetter(_nodeVBO, _dStore.getNodeStructureInfo()->getAllNodes(_dStore.getNodeStructureInfo()->getMaxDepth()));

	if (!silent) {
		changeData();
	}
}	


glm::mat4 Renderer::getStandardMVP( void ) {
	glm::mat4 P = cameraHelper::calculateProjection(_dStore.getNodeStructureInfo(), context::_zoomFactor);
	glm::mat4 MVP = glm::translate(P, glm::vec3(context::_worldTransX, context::_worldTransY, 0.0f));

	return MVP;
}

void Renderer::renderGraph(IRenderData* rData, int xMove, int yMove) {
	int mouseMoveX, mouseMoveY;
	mouseHandler::getPressMovement(&mouseMoveX, &mouseMoveY);

	mouseMoveX = mouseMoveX + xMove;
	mouseMoveY = mouseMoveY + yMove;

	float moveX, moveY;
	cameraHelper::mouseDist2StandardVolDist(&moveX, &moveY, mouseMoveX, mouseMoveY);
	float maxValues[] = {1.0f, 1.0f, 1.0f}; //vis tex is already normalized

	renderTexture(rData->getVis(), maxValues, moveX, moveY);
	renderLabels(rData, mouseMoveX, -mouseMoveY);
}

void Renderer::renderHUD(float progress, int pBars, float maxVals[]) {
	if (progress >= 0.0f) {
		_progressBar.renderBar(progress, pBars);
	}

	if (maxVals[0] > 0.0f) {
		_scalingBars.renderScaleBars(maxVals, _hasEdges);
	}
}

void Renderer::renderLabelSelection(IRenderData* rData, int xMove, int yMove) {
	int xShift, yShift;
	mouseHandler::getPressMovement(&xShift, &yShift);
	_labelSelectionPainter.renderSelection(getStandardMVP(), rData->getEvalField(), xShift, yShift, xMove, yMove);
}

void Renderer::renderLabels(IRenderData* rData, int xMove, int yMove) {
	if (_dStore.hasLabels() && 	context::_options._showLabels)  {
		_labelPainter.renderLabels(xMove, yMove);
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
 	context::_pixelSize = cameraHelper::getPixelSize(_dStore.getNodeStructureInfo(), context::_zoomFactor);
	_labelSelectionPainter.clear();
	_state->changeZoom();
}

void Renderer::changeData( void ) {
	context::_pixelSize = cameraHelper::getPixelSize(_dStore.getNodeStructureInfo(), context::_zoomFactor);
	context::_options._lock = false;
	_labelSelectionPainter.clear();
	_labelPainter.clear();
	_state->changeData();
}

void Renderer::changeSideLength( void ) {
	_labelSelectionPainter.clear();
	_state->changeSideLength();
}

void Renderer::changeWindow( void ) {
	context::getWindowSize(&_windowWidth, &_windowHeight);
	context::_pixelSize = cameraHelper::getPixelSize(_dStore.getNodeStructureInfo(), context::_zoomFactor);
	_labelSelectionPainter.resize(_windowWidth, _windowHeight);
	_labelSelectionPainter.clear();
	_state->changeWindow();
}

void Renderer::changeVisParameter( void ) {
	_state->changeVisParameter();
}