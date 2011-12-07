#include "Renderer.hpp"
#include "../GlobalConstants.hpp"
#include "../context/Context.hpp"
#include "../helper/CameraHelper.hpp"
#include "../helper/MouseHandler.hpp"
#include "../Node.hpp"


Renderer::Renderer( void ) {
	_currentData = new RenderData;
	_newData = new RenderData;

	glGenBuffers(1, &_nodeVBO);
	context::getWindowSize(&_windowWidth, &_windowHeight);

	//load initial data
	setNewData("_Data/LineNode.out");
	context::_pixelSize = cameraHelper::getPixelSize(dCache.getNodeStructureInfo(), context::_zoomFactor);

	_idle = new Idle(this);
	_initalWork = new InitialWork(this);
	_working = new Working(this);
	setState(_initalWork);
}

Renderer::~Renderer( void ) {
	glDeleteBuffers(1, &_nodeVBO);

	delete _currentData;
	delete _newData;
	delete _idle;
	delete _initalWork;
	delete _working;
}


//public


//private methods that can be used by the states

void Renderer::setState(IRenderState* state) {
	_state = state;
	_state->takeOver();
}

void Renderer::setNewData(string nodeFile, string edgeFile) {
	dCache.loadDataSet(nodeFile, edgeFile);

	int nodeCount;
	const PackedNode* packedNodes = dCache.getPackedNodes(&nodeCount);

	glBindBuffer(GL_ARRAY_BUFFER, _nodeVBO);
		glBufferData(GL_ARRAY_BUFFER, nodeCount * sizeof(PackedNode), packedNodes, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}	

void Renderer::renderGraph(RenderData* rData) {
	int mouseMoveX, mouseMoveY;
	mouseHandler::getPressMovement(&mouseMoveX, &mouseMoveY);

	float moveX, moveY;
	cameraHelper::mouseDist2StandardVolDist(&moveX, &moveY, mouseMoveX, mouseMoveY);

	_visPainter.renderVis(rData, moveX, moveY);
}

void Renderer::renderHUD(float progress) {
	_progressBar.renderBar(progress);
}

void Renderer::renderTexture(GLuint tex, float rMax, float gMax, float bMax) {
	_displayConvert.renderTexture(tex, rMax, gMax, bMax);
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

void Renderer::work( void ) {
	_state->work();
}

void Renderer::takeOver( void ) {
	//state only 
}

void Renderer::changePanning( void ) {
	_state->changePanning();
}

void Renderer::changeZoom( void ) {
 	context::_pixelSize = cameraHelper::getPixelSize(dCache.getNodeStructureInfo(), context::_zoomFactor);
	_state->changeZoom();
}

void Renderer::changeData(string nodeFile, string edgeFile) {
	setNewData(nodeFile, edgeFile);
	context::_pixelSize = cameraHelper::getPixelSize(dCache.getNodeStructureInfo(), context::_zoomFactor);
	_state->changeData(nodeFile, edgeFile);
}

void Renderer::changeSideLength( void ) {
	_state->changeSideLength();
}

void Renderer::changeWindow( void ) {
	context::getWindowSize(&_windowWidth, &_windowHeight);
	context::_pixelSize = cameraHelper::getPixelSize(dCache.getNodeStructureInfo(), context::_zoomFactor);
	_state->changeWindow();
}