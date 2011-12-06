#include "Renderer.hpp"
#include "../GlobalConstants.hpp"
#include "../context/Context.hpp"
#include "../Node.hpp"


Renderer::Renderer( void ) {
	_currentData = new RenderData;
	_newData = new RenderData;

	glGenBuffers(1, &_nodeVBO);
	context::getWindowSize(&_windowWidth, &_windowHeight);
	
	_idle = new Idle(this);
	_initalWork = new InitialWork(this);
	_working = new Working(this);
	_state = _initalWork;

	//load initial data
	setNewData("_Data/WorkNode.out");
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

void Renderer::setNewData(string nodeFile, string edgeFile) {
	dCache.loadDataSet(nodeFile, edgeFile);

	int nodeCount;
	const PackedNode* packedNodes = dCache.getPackedNodes(&nodeCount);

	glBindBuffer(GL_ARRAY_BUFFER, _nodeVBO);
		glBufferData(GL_ARRAY_BUFFER, nodeCount * sizeof(PackedNode), packedNodes, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	changeData();
}	

//private methods that can be used by the states

void Renderer::renderGraph( void ) {

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

void Renderer::changePanning( void ) {
	_state->changePanning();
}

void Renderer::changeZoom( void ) {
	_state->changeZoom();
}

void Renderer::changeData( void ) {
	_state->changeData();
}

void Renderer::changeAspectRatio( void ) {
	_state->changeAspectRatio();
}