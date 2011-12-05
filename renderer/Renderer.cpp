#include "Renderer.hpp"
#include "../painter/ProgressbarPainter.hpp"
#include "../Node.hpp"


Renderer::Renderer( void ) {
	_currentData = NULL;
	_newData = NULL;

	glGenBuffers(1, &_nodeVBO);

	//load data
	setNewData("_Data/WorkNode.out");
	
	_idle = new Idle(this);
	_initalWork = new InitialWork(this);
	_working = new Working(this);
	_state = _initalWork;
}

Renderer::~Renderer( void ) {
	delete _currentData;
	delete _newData;
	delete _idle;
	delete _initalWork;
	delete _working;
}


//private methods that can be used by the states

void Renderer::setNewData(string nodeFile, string edgeFile) {
	dCache.loadDataSet(nodeFile, edgeFile);

	int nodeCount;
	const PackedNode* packedNodes = dCache.getPackedNodes(&nodeCount);

	glBindBuffer(GL_ARRAY_BUFFER, _nodeVBO);
		glBufferData(GL_ARRAY_BUFFER, nodeCount * sizeof(PackedNode), packedNodes, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}	

void Renderer::renderGraph( void ) {

}

void Renderer::renderHUD(float progress) {
	_progressBar.renderBar(progress);
}

void Renderer::renderTexture(GLuint tex, float rMax, float gMax, float bMax) {
	_displayConvert.renderTexture(tex, rMax, gMax, bMax);
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